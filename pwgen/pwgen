#! /bin/bash
#pwgen: search a file encrypted with aescrypt (aescrypt.com/download.html) for a username/password pair

#Simulates keystrokes in seq arg2 to window with name arg1
print_to_window () {
  #param1: window title
  #param2: password

  #See if there are more than one window with title == $1
  if [ $(xdotool search --name $1 | wc -l) -gt 1 ]
  then
    echo More than one window with $1 in title
    return
  fi

  xdotool windowfocus $(xdotool search --name $1)
  xdotool type $2
}

#Keychain locations
KEYCHAIN_DECRYPTED=$XDG_DATA_HOME/pwdb/keychain.kcn
KEYCHAIN_ENCRYPTED=$XDG_DATA_HOME/pwdb/keychain.aes

#Get password and domain (pw for facebook.com, mybank.com, etc)
#This isn't secure, so make sure your environment is!
read -s -p 'Password: ' PWORD
echo -e
read -p 'Domain: ' DOMAIN

aescrypt -d -p $PWORD -o $KEYCHAIN_DECRYPTED $KEYCHAIN_ENCRYPTED

if [ ! -f $KEYCHAIN_DECRYPTED ]
then
  #aescrypt will print an appropriate error statement
  #so we don't have to
  exit
fi

FOUND=0
DPASS=$(grep "$DOMAIN " $KEYCHAIN_DECRYPTED | awk '{print $2}' | cut -c -12)

if [ ! -z $DPASS ]
then
  FOUND=1
  echo $DPASS
  
  #Echo $DPASS to our Firefox window, if it's open
  print_to_window Mozilla $DPASS

fi

if [ $FOUND = 0 ]
then
  #Ensure we are working with a decrypted copy
  if [ -e $KEYCHAIN_DECRYPTED ]
  then
    read -p "Write new password for $DOMAIN (Y/n)? " WRITE
    if [ 'n' != $WRITE -a 'N' != $WRITE ]
    then

      DPASS=`newpw 12`
      echo $DOMAIN $DPASS >> $KEYCHAIN_DECRYPTED
      aescrypt -e -p $PWORD -o $KEYCHAIN_ENCRYPTED $KEYCHAIN_DECRYPTED

      echo 'Wrote new password for' $DOMAIN
      echo $DPASS

    fi
  fi
fi

#Erase unencrypted pw file
rm $KEYCHAIN_DECRYPTED
