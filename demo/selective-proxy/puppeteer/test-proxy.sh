#!/usr/bin/env bash
proxy=$1 # http://user:pass@hostname:port

# Break it down


user=$(echo $proxy | sed 's/http:\/\///g' | cut -d ':' -f1)
pass=$(echo $proxy | sed 's/http:\/\///g' | cut -d ':' -f2 | cut -d '@' -f1)
host=$(echo $proxy | sed 's/http:\/\///g' | cut -d '@' -f2 | cut -d ':' -f1)
port=$(echo $proxy | sed 's/http:\/\///g' | cut -d '@' -f2 | cut -d ':' -f2)




curl -x $host:$port -U $user:$pass https://api.ipify.org?format=json
