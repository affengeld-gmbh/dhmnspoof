#gcc -DHAVE_CONFIG_H -fPIC -I. -I../includes  -DCLIENT_PATH='"PATH=/usr/sbin:/sbin:/bin:/usr/sbin:/usr/bin"' -DLOCALSTATEDIR='"/var"'   -g -O2  -Wall -Werror -fno-strict-aliasing -I../includes -I/root/iscdhcp/dhcp-4.4.1/bind/include -MT dhclient.o -MD -MP -MF .deps/dhclient.Tpo -c -o dhclient.o dhclient.c

#gcc -fPIC -g -O2  -Wall -Werror -fno-strict-aliasing -I../includes -I/root/iscdhcp/dhcp-4.4.1/bind/include   -o dhclient client_tables.o clparse.o dhclient.o dhc6.o ../common/libdhcp.a ../omapip/libomapi.a /root/iscdhcp/dhcp-4.4.1/bind/bind-9.11.2-P1/lib/irs/libirs.a /root/iscdhcp/dhcp-4.4.1/bind/bind-9.11.2-P1/lib/dns/libdns.a /root/iscdhcp/dhcp-4.4.1/bind/bind-9.11.2-P1/lib/isccfg/libisccfg.a /root/iscdhcp/dhcp-4.4.1/bind/bind-9.11.2-P1/lib/isc/libisc.a 


gcc -DHAVE_CONFIG_H -I. -I../includes  -DCLIENT_PATH='"PATH=/usr/sbin:/sbin:/bin:/usr/sbin:/usr/bin"' -DLOCALSTATEDIR='"/var"'   -g -O2  -Wall -Werror -fno-strict-aliasing -I../includes -I/root/iscdhcp/dhcp-4.4.1/bind/include -MT dhclient.o -MD -MP -MF .deps/dhclient.Tpo -c -o dhclient.o dhclient.c -fPIC -no-pie

gcc  -g -O2  -Wall -Werror -fno-strict-aliasing -I../includes -I/root/iscdhcp/dhcp-4.4.1/bind/include   -o dhclient client_tables.o clparse.o dhclient.o dhc6.o ../common/libdhcp.a ../omapip/libomapi.a /root/iscdhcp/dhcp-4.4.1/bind/bind-9.11.2-P1/lib/irs/libirs.a /root/iscdhcp/dhcp-4.4.1/bind/bind-9.11.2-P1/lib/dns/libdns.a /root/iscdhcp/dhcp-4.4.1/bind/bind-9.11.2-P1/lib/isccfg/libisccfg.a /root/iscdhcp/dhcp-4.4.1/bind/bind-9.11.2-P1/lib/isc/libisc.a -fPIC -no-pie
