#!/bin/bash

mount -o remount,rw /

mellon-cli -d /dev/ttyUSB0 setTime > /dev/null 2>&1

ldapUser="mellon.hgcloud"
#Note: Serial device needs to be explicitely specified to work from cron.
#ldapPassword="$(mellon-cli -d /dev/ttyUSB0 aesDecryptString 0 86ECD0F7A19D163EDA3965B39E547B3640E774AF7BB5DFA84706857B1E3CAD3E476B855F038F36AFBD5E4BBAC1975CE0E6B5D76333B903CD0AFCB0EC085CA481DEFCC18F4ED0F176C3284FCDBCFDA1BF298CF523D82DF58E36F9410B28A9B7E9)"
echo "$(($(date +%s%N)/1000000)) Decrypting LDAP password (2)..." >> /data/mellon-cli.log
ldapPassword="$(mellon-cli -d /dev/ttyUSB0 aesDecryptString 0 86ECD0F7A19D163EDA3965B39E547B3640E774AF7BB5DFA84706857B1E3CAD3E476B855F038F36AFBD5E4BBAC1975CE0E6B5D76333B903CD0AFCB0EC085CA481DEFCC18F4ED0F176C3284FCDBCFDA1BF298CF523D82DF58E36F9410B28A9B7E9)"
echo "$(($(date +%s%N)/1000000)) Decrypted LDAP password (2)." >> /data/mellon-cli.log
ldapBase="OU=MyBusiness,DC=ELLI,DC=local"

lastSync=$(date +%s)

IFS=
DNS=`ldapsearch -x -o ldif-wrap=no -b "$ldapBase" -D "$ldapUser" -w "$ldapPassword" -LLL -s sub "(&(objectclass=user)(userAccountControl:1.2.840.113556.1.4.803:=512)(!(userAccountControl:1.2.840.113556.1.4.803:=2)))" dn`
if [ $? -ne 0 ]; then
	echo "Error getting users"
	exit 1
fi
IFS='\n'; readarray -t DNS2 <<< $DNS; IFS=
for entry in ${DNS2[@]}; do
	user=""
	uidNumber=""
	gidNumber=""
	ibSshPublicCertificate=""
	hasMembers=0
	if [[ "$entry" =~ ^dn:: ]]; then
		distinguishedName=${entry:5} && distinguishedName=$(echo $distinguishedName | base64 -d)
	else
		distinguishedName=${entry:4}
	fi

	echo "Checking $distinguishedName..."
	result=`ldapsearch -x -o ldif-wrap=no -b "$distinguishedName" -D "$ldapUser" -w "$ldapPassword" -LLL -s sub "(&(objectclass=user)(userAccountControl:1.2.840.113556.1.4.803:=512)(!(userAccountControl:1.2.840.113556.1.4.803:=2)))" memberOf uidNumber gidNumber ibSshPublicCertificate sAMAccountName`
	if [ $? -ne 0 ]; then
	        echo "Error getting user"
        	exit 1
	fi
	IFS='\n'; readarray -t DNA <<< $result; IFS=
	for entry in ${DNA[@]}; do
		if [[ "$entry" =~ ^uidNumber: ]]; then
			uidNumber=${entry:11}
		elif [[ "$entry" =~ ^gidNumber: ]]; then
			gidNumber=${entry:11}
		elif [[ "$entry" =~ ^sAMAccountName: ]]; then
			user=${entry:16}
		elif [[ "$entry" =~ ^ibSshPublicCertificate: ]]; then
			ibSshPublicCertificate=${entry:24}
			if [[ "$ibSshPublicCertificate" =~ ^ssh-rsa ]]; then
				ibSshPublicCertificate=${ibSshPublicCertificate:8}
			fi
		elif [[ "$entry" =~ ^memberOf: ]]; then
			if [[ "$entry" =~ OU=Mellon\ Principals,OU=ib\ company\ GmbH,OU=MyBusiness,DC=ELLI,DC=local$ ]]; then
				hasMembers=1
			fi
		fi
	done

	if [ ! -z $user ] && [ ! -z $uidNumber ] && [ ! -z $gidNumber ] && [ ! -z $ibSshPublicCertificate ] && [ $hasMembers -eq 1 ]; then
		user=${user//[\/\0]/}
		if [ ! -d /home/$user ]; then
			mkdir -p /home/$user/.ssh
			chown $uidNumber:root -R /home/$user
			echo $distinguishedName > /home/$user/distinguishedName
			echo "ssh-rsa $ibSshPublicCertificate" > /home/$user/.ssh/authorized_keys
			usermod -a -G dialout $user
			touch /home/$user/lastLdapUpdate
		else
			currentCert=$(cat /home/$user/.ssh/authorized_keys)
			if [ "$currentCert" != "ssh-rsa $ibSshPublicCertificate" ]; then
				echo "ssh-rsa $ibSshPublicCertificate" > /home/$user/.ssh/authorized_keys
			fi
		fi
	fi
done

deleteBefore=$((lastSync-86400))
for directory in /home/*/; do
	[[ -f ${directory}noLdapSync ]] && continue
	if [ -f ${directory}lastLdapUpdate ]; then
		lastUpdate=$(stat -c %Y ${directory}lastLdapUpdate)
	else
		rm -Rf $directory
		continue
	fi
	if [ $lastUpdate -lt $deleteBefore ]; then
		rm -Rf $directory
	fi
done

sync
sleep 5
mount -o remount,ro /
