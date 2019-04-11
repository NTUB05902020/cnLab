#!/bin/bash/

for i in $(sudo iptables -t nat --line-numbers -L | grep ".*tcp" | awk '{print $1}' | tac);
do
	sudo iptables -t nat -D PREROUTING $i;
done
sudo iptables -t nat -A PREROUTING -i wlx74da38db1b32 -p tcp --dport 80 -j DNAT --to-destination 10.0.2.15:8888
sudo iptables -t nat -A PREROUTING -i wlx74da38db1b32 -p tcp --dport 443 -j DNAT --to-destination 10.0.2.15:8888
