#!/bin/bash

if [ "$1" = "-c" ]; then
	if [[ "$2" =~ ^aesEncryptString  ]]; then
		string=${2:17}
		#/usr/bin/mellon-cli -d /dev/serial/by-id/usb-FTDI_FT230X_Basic_UART_DM01LDGH-if00-port0 aesEncryptString 0 -- "$string" < /dev/null
		echo "$(($(date +%s%N)/1000000)) Encrypting string with AES..." >> /data/mellon-cli.log
		/usr/bin/mellon-cli -d /dev/ttyUSB0 aesEncryptString 0 -- "$string" < /dev/null
		echo "$(($(date +%s%N)/1000000)) Encrypted string with AES." >> /data/mellon-cli.log
	elif [[ "$2" =~ ^aesDecryptString  ]]; then
		string=${2:17}
		#/usr/bin/mellon-cli -d /dev/serial/by-id/usb-FTDI_FT230X_Basic_UART_DM01LDGH-if00-port0 aesDecryptString 0 -- "$string" < /dev/null
		echo "$(($(date +%s%N)/1000000)) Decrypting string with AES..." >> /data/mellon-cli.log
		/usr/bin/mellon-cli -d /dev/ttyUSB0 aesDecryptString 0 -- "$string" < /dev/null
		echo "$(($(date +%s%N)/1000000)) Decrypted string with AES." >> /data/mellon-cli.log
	fi
	exit 0
fi

ldapUser="mellon.hgcloud"
#ldapPassword="$(mellon-cli -d /dev/ttyUSB0 aesDecryptString 0 86ECD0F7A19D163EDA3965B39E547B3640E774AF7BB5DFA84706857B1E3CAD3E476B855F038F36AFBD5E4BBAC1975CE0E6B5D76333B903CD0AFCB0EC085CA481DEFCC18F4ED0F176C3284FCDBCFDA1BF298CF523D82DF58E36F9410B28A9B7E9)"
echo "$(($(date +%s%N)/1000000)) Decrypting LDAP password..." >> /data/mellon-cli.log
ldapPassword="$(mellon-cli -d /dev/ttyUSB0 aesDecryptString 0 86ECD0F7A19D163EDA3965B39E547B3640E774AF7BB5DFA84706857B1E3CAD3E476B855F038F36AFBD5E4BBAC1975CE0E6B5D76333B903CD0AFCB0EC085CA481DEFCC18F4ED0F176C3284FCDBCFDA1BF298CF523D82DF58E36F9410B28A9B7E9)"
echo "$(($(date +%s%N)/1000000)) Decrypted LDAP password." >> /data/mellon-cli.log
ldapBase="OU=MyBusiness,DC=ELLI,DC=local"
distinguishedName=$(cat ~/distinguishedName)

function joinArray { local IFS="$1"; shift; echo "$*"; }

loggedInUser=$(getent passwd $(id -u) | cut -d ":" -f 1)
[[ -z $loggedInUser ]] && exit 1

echo "$(($(date +%s%N)/1000000)) User $loggedInUser successfully logged in and is retrieving SSH certificate." >> /data/mellon-cli.log

IFS=
result=`ldapsearch -x -o ldif-wrap=no -b "$distinguishedName" -D "$ldapUser" -w "$ldapPassword" -LLL -s sub "(&(objectClass=user)(userAccountControl:1.2.840.113556.1.4.803:=512)(!(userAccountControl:1.2.840.113556.1.4.803:=2)))" memberOf uidNumber gidNumber sAMAccountName ibSshPublicCertificate`
IFS='\n'; readarray -t DNA <<< $result; IFS=
memberOf=()
for entry in ${DNA[@]}; do
	if [[ "$entry" =~ ^uidNumber: ]]; then
		uidNumber=${entry:11}
	elif [[ "$entry" =~ ^sAMAccountName: ]]; then
		user=${entry:16}
	elif [[ "$entry" =~ ^ibSshPublicCertificate: ]]; then
		ibSshPublicCertificate=${entry:24}
		if [[ "$ibSshPublicCertificate" =~ ^ssh-rsa ]]; then
			ibSshPublicCertificate=${ibSshPublicCertificate:8}
		fi
		ibSshPublicCertificate=$(echo $ibSshPublicCertificate | sed 's/[^0-9A-Za-z+\/=]*//g')
	elif [[ "$entry" =~ ^memberOf: ]]; then
		if [[ "$entry" =~ OU=Mellon\ Principals,OU=ib\ company\ GmbH,OU=MyBusiness,DC=ELLI,DC=local$ ]]; then
			memberDn=${entry:10}
			memberOf+=(`ldapsearch -x -o ldif-wrap=no -b "$memberDn" -D "$ldapUser" -w "$ldapPassword" -LLL -s sub "(objectclass=group)" info | grep "^info:" | cut -d " " -f 2- | tr -d '[:space:]'`)
		fi
	fi
done

[[ $uidNumber -ne $(id -u) ]] && exit 1
[[ -z $ibSshPublicCertificate ]] && exit 1
[[ "$user" != "$loggedInUser" ]] && exit 1

principals=$(joinArray , "${memberOf[@]}")

#/usr/bin/mellon-cli -d /dev/serial/by-id/usb-FTDI_FT230X_Basic_UART_DM01LDGH-if00-port0 signUserCert 0 $ibSshPublicCertificate "$user" "$principals"
echo "$(($(date +%s%N)/1000000)) Signing user certificate $ibSshPublicCertificate $user $principals..." >> /data/mellon-cli.log
/usr/bin/mellon-cli -d /dev/ttyUSB0 signUserCert 0 $ibSshPublicCertificate "$user" "$principals"
echo "$(($(date +%s%N)/1000000)) Signed user certificate." >> /data/mellon-cli.log
