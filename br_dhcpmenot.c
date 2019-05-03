/*
 *	Telus & NorthWestTel DHCPMeNot! Plugin Module v1.56
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Achim Kong			<achim.kong@bc.geoterra.net>
 *
 *	This program contains proprietary code and is the intellectual	
 *	property of the author. It may not be redistributed, used, nor	
 *	modified without prior written consent.	
 *	
 */

#include "br_dhcpmenot.h"
#define MAXENTRIES 32 
#define NETLINK_USER 31
#define BRNOMATMATCH 69

struct sock *nl_sk = NULL;
matdb records[MAXENTRIES];

static ipl3hdr br_ip_hex_deobfuscate(unsigned short optcode, struct sk_buff *skb)
{
	ipl3hdr myL3Hdr;
	struct iphdr *ipkopf = (struct iphdr *)skb_network_header(skb);
	struct arpscheisskopf *arpkopf=(struct arpscheisskopf *)arp_hdr(skb);
	unsigned int a,b,c,d,o;
	char L3IP[8];

	switch(optcode){
	case 0:
		for(o=4; o!=0; o-=2){
			myL3Hdr.ipaddrsrc[o-1]=arpkopf->ar_sip[o-1];
			myL3Hdr.ipaddrsrc[o-2]=arpkopf->ar_sip[o-2];
					
			myL3Hdr.ipaddrdst[o-1]=arpkopf->ar_tip[o-1];
			myL3Hdr.ipaddrdst[o-2]=arpkopf->ar_tip[o-2];
		}
		break;
	case 1:
		sprintf(L3IP,"%08lx",(unsigned long)ntohl(ipkopf->saddr));
		sscanf(L3IP, "%2x%2x%2x%2x", &a, &b, &c, &d); 

		myL3Hdr.ipaddrsrc[0]=a;
		myL3Hdr.ipaddrsrc[1]=b;
		myL3Hdr.ipaddrsrc[2]=c;
		myL3Hdr.ipaddrsrc[3]=d;

		sprintf(L3IP,"%08lx",(unsigned long)ntohl(ipkopf->daddr));
		sscanf(L3IP, "%2x%2x%2x%2x", &a, &b, &c, &d);

		myL3Hdr.ipaddrdst[0]=a;
		myL3Hdr.ipaddrdst[1]=b;
		myL3Hdr.ipaddrdst[2]=c;
		myL3Hdr.ipaddrdst[3]=d;				
	}
	return myL3Hdr;
}

static unsigned short br_mat_match(unsigned short ipType, ipl3hdr myL3Hdr){
	unsigned short o, matdbid, octetsMatched=0;
	switch(ipType){
		/*
		 * CASE 0 FOR SOURCE
		 */
	   case 0:	     
            for (matdbid=0; matdbid<records[0].numentries; matdbid++){
                for(o=0; o<4; o+=2){
			if(myL3Hdr.ipaddrsrc[o]==records[matdbid].ip[o])
				octetsMatched++;
			else
				break;
                	if(myL3Hdr.ipaddrsrc[o+1]==records[matdbid].ip[o+1])
                       		octetsMatched++;
                }
                if(octetsMatched==4)
			return matdbid;
                else 
                    octetsMatched=0;     
	    }
	    break;

	   case 1:
	    /*
	     * CASE 1 FOR DST
	     */	     
            for (matdbid=0; matdbid<records[0].numentries; matdbid++){	     
                for(o=0; o<4; o+=2){
                    if(myL3Hdr.ipaddrdst[o]==records[matdbid].ip[o])
						octetsMatched++;
					else
						break;	
                    if(myL3Hdr.ipaddrdst[o+1]==records[matdbid].ip[o+1])
						octetsMatched++;
                }
            if(octetsMatched==4){	     		
				return matdbid;
			} else  
                octetsMatched=0;
		}
		break;
	}
	return BRNOMATMATCH;
}

void br_telus_mat(struct sk_buff *skb){
    struct ethhdr *ethhdr=eth_hdr(skb);
	struct arpscheisskopf *arpkopf;
	unsigned short ne, neSRC, neDST, matdbid, matdbidSrc=0, matdbidDst=0, dyniMac=1; //dyniMac is temporary placeholder
	unsigned short counterSrc=0, counterDst=0;
	ipl3hdr myL3Hdr;

	switch(ntohs(ethhdr->h_proto)){
		case ETH_P_ARP:		
			arpkopf=(struct arpscheisskopf *)arp_hdr(skb);
			myL3Hdr = br_ip_hex_deobfuscate(0,skb);

			if((matdbid = br_mat_match(0,myL3Hdr))!=BRNOMATMATCH){
				for(ne=0;ne<6;ne++){ //Loop ID 2	
					if((dyniMac==1)&&(ethhdr->h_source[ne])!=(records[matdbid].imac[ne])){
						//As soon as a non-matching octet is found, save the dynamic iMAC to the matdb
						printk(KERN_INFO "DHMNv1.56> Dynamic iMAC in ARP Process Recorded: ");
						printk(KERN_INFO "DHMNv1.56> %lx:%lx:%lx:%lx:%lx:%lx ==>> %lx:%lx:%lx:%lx:%lx:%lx\n", records[matdbid].imac[0],
															records[matdbid].imac[1],	
															records[matdbid].imac[2],
															records[matdbid].imac[3],
															records[matdbid].imac[4],
															records[matdbid].imac[5],
															ethhdr->h_source[0],
															ethhdr->h_source[1],
															ethhdr->h_source[2],
															ethhdr->h_source[3],
															ethhdr->h_source[4],
															ethhdr->h_source[5]);
																	

						for(neSRC=6;neSRC!=0;neSRC-=3){
							//Store the dynamic iMAC into the database before changing it to the eMAC
							records[matdbid].imac[neSRC-1]=ethhdr->h_source[neSRC-1];
							records[matdbid].imac[neSRC-2]=ethhdr->h_source[neSRC-2];
							records[matdbid].imac[neSRC-3]=ethhdr->h_source[neSRC-3];
						}
						//we are done!
						break;
					}
				}

				for(ne=6;ne!=0;ne-=3){
					arpkopf->ar_sha[ne-1]=records[matdbid].emac[ne-1];
					arpkopf->ar_sha[ne-2]=records[matdbid].emac[ne-2];
					arpkopf->ar_sha[ne-3]=records[matdbid].emac[ne-3];
					
					ethhdr->h_source[ne-1]=records[matdbid].emac[ne-1];
					ethhdr->h_source[ne-2]=records[matdbid].emac[ne-2];			
					ethhdr->h_source[ne-3]=records[matdbid].emac[ne-3];
				}
			} else if(((matdbid=br_mat_match(1,myL3Hdr))!=BRNOMATMATCH)&&((ethhdr->h_dest[0]!=255) && 
											(ethhdr->h_dest[5]!=255))){
				for(ne=6;ne!=0;ne-=3){
					arpkopf->ar_tha[ne-1]=records[matdbid].imac[ne-1];
					arpkopf->ar_tha[ne-2]=records[matdbid].imac[ne-2];
					arpkopf->ar_tha[ne-3]=records[matdbid].imac[ne-3];
					
					ethhdr->h_dest[ne-1]=records[matdbid].imac[ne-1];
					ethhdr->h_dest[ne-2]=records[matdbid].imac[ne-2];
					ethhdr->h_dest[ne-3]=records[matdbid].imac[ne-3];
				}
			}
			break;
		case ETH_P_IP:
			myL3Hdr = br_ip_hex_deobfuscate(1,skb);

			/*
			 * ROAST MAT Routine
			 */
			matdbidSrc=br_mat_match(0,myL3Hdr);
			matdbidDst=br_mat_match(1,myL3Hdr);

			if((matdbidSrc!=BRNOMATMATCH) || (matdbidDst!=BRNOMATMATCH)){
				for(ne=0;ne<6;ne++){ //Loop ID 0
					if((ethhdr->h_dest[ne])==(records[matdbidDst].emac[ne])){ 
						counterDst++;
						if(counterDst==6){
							for(neDST=6;neDST!=0;neDST-=3){ //Loop ID 1
								ethhdr->h_dest[neDST-1]=records[matdbidDst].imac[neDST-1];
								ethhdr->h_dest[neDST-2]=records[matdbidDst].imac[neDST-2];
								ethhdr->h_dest[neDST-3]=records[matdbidDst].imac[neDST-3];
							}
							//we are done no need to check ROAST Source
							break;
						}
					} else {
						for(ne=0;ne<6;ne++){ //Loop ID 2	
							if((ethhdr->h_source[ne])==(records[matdbidSrc].imac[ne])){
								counterSrc++;
								if(counterSrc==6){
									for(neSRC=6;neSRC!=0;neSRC-=3){ //Loop ID 3
										ethhdr->h_source[neSRC-1]=records[matdbidSrc].emac[neSRC-1];
										ethhdr->h_source[neSRC-2]=records[matdbidSrc].emac[neSRC-2];
										ethhdr->h_source[neSRC-3]=records[matdbidSrc].emac[neSRC-3];
									}
									//we are done with ROAST Source ne is 5 we break to prevent additional cycle
									break;
								}
							} else {
								//	//at 1st non-matching octet, quite the middle for loop and go onto next packet
								// Check if dynamic iMAC is enabled, if so, run routine
								if(dyniMac==1){
									for(neSRC=6;neSRC!=0;neSRC-=3){
										//Store the dynamic iMAC into the database before changing it to the eMAC
										records[matdbidSrc].imac[neSRC-1]=ethhdr->h_source[neSRC-1];
										records[matdbidSrc].imac[neSRC-2]=ethhdr->h_source[neSRC-2];
										records[matdbidSrc].imac[neSRC-3]=ethhdr->h_source[neSRC-3];
										//MAT the source with eMAC
                                                                		ethhdr->h_source[neSRC-1]=records[matdbidSrc].emac[neSRC-1];
                                                                		ethhdr->h_source[neSRC-2]=records[matdbidSrc].emac[neSRC-2];
                                                                		ethhdr->h_source[neSRC-3]=records[matdbidSrc].emac[neSRC-3];
									}
									printk(KERN_INFO "DHMNv1.56> Dynamic iMAC in ETH_P_IP Process Recorded: ");
                                                printk(KERN_INFO "DHMNv1.56> %lx:%lx:%lx:%lx:%lx:%lx ==>> %lx:%lx:%lx:%lx:%lx:%lx\n", records[matdbidSrc].imac[0],
                                                                                                                        records[matdbidSrc].imac[1],
                                                                                                                        records[matdbidSrc].imac[2],
                                                                                                                        records[matdbidSrc].imac[3],
                                                                                                                        records[matdbidSrc].imac[4],
                                                                                                                        records[matdbidSrc].imac[5],
                                                                                                                        ethhdr->h_source[0],
                                                                                                                        ethhdr->h_source[1],
                                                                                                                        ethhdr->h_source[2],
                                                                                                                        ethhdr->h_source[3],
                                                                                                                        ethhdr->h_source[4],
                                                                                                                        ethhdr->h_source[5]);
								}
								break;
							}
						}
						//break the master loop
						break;		
					}
				}	
			}
	}
}

static void br_read_mat_recv_msg(struct sk_buff *skb){
	struct nlmsghdr *nlh;
	struct sk_buff *skb_out;
	unsigned int pid, msg_size, res, ne; 
	char *msg="DHMNv1.56:br_read_mat_recv_msg> Kernel Module Reponse - MatDB Import Succeeded!";

	msg_size=strlen(msg);
	nlh=(struct nlmsghdr*)skb->data;

	memcpy(records,(matdb*)nlmsg_data(nlh),8);
	memcpy(records,(matdb*)nlmsg_data(nlh),(unsigned long)records[0].dbsize);

	printk(KERN_INFO "DHMNv1.56:br_init> (%s) Size of MatDB Received: %i\n", __FUNCTION__, (int)records[0].dbsize);

	for(ne=0; ne<records[0].numentries; ne++){
		printk(KERN_INFO "DHMNv1.56:br_init> MatDBID: %i\n", ne);
		printk(KERN_INFO "DHMNv1.56:br_init> IP: %lu.%lu.%lu.%lu\n", records[ne].ip[0],records[ne].ip[1],
																	 records[ne].ip[2],records[ne].ip[3]);


		printk(KERN_INFO "DHMNv1.56:br_init> eMAC:%.2lx-%.2lx-%.2lx-%.2lx-%.2lx-%.2lx\n",  records[ne].emac[0], 
                                                                		records[ne].emac[1], 
                                                                		records[ne].emac[2], 
                                                                		records[ne].emac[3], 
                                                                		records[ne].emac[4], 
                                                                		records[ne].emac[5]);

		printk(KERN_INFO "DHMNv1.56:br_init> iMAC:%.2lx-%.2lx-%.2lx-%.2lx-%.2lx-%.2lx\n\n", records[ne].imac[0], 
                                                                		 records[ne].imac[1], 
                                                                		 records[ne].imac[2], 
                                                                		 records[ne].imac[3], 
                                                                		 records[ne].imac[4], 
                                                                		 records[ne].imac[5]);
	}
	pid = nlh->nlmsg_pid;
	skb_out = nlmsg_new(msg_size,0);

	if(!skb_out){
    		printk(KERN_ERR "DHMNv1.56:br_init> Failed to allocate new skb, MatDB may not have been imported properly!\n");
    	return;
	} 

	nlh=nlmsg_put(skb_out,0,0,NLMSG_DONE,msg_size,0);  
	NETLINK_CB(skb_out).dst_group = 0; 
	strncpy(nlmsg_data(nlh),msg,msg_size);
	res=nlmsg_unicast(nl_sk,skb_out,pid);

	if(res<0)
	printk(KERN_INFO "DHMNv1.56:br_init> Error while reporting status back to brmatcfg!\n");
}

// ************* KERNEL MODULE ENTRY POINT (Called by br.c) *****************
unsigned int br_read_mat_init(void){
	struct netlink_kernel_cfg cfg = {
    		.input = br_read_mat_recv_msg,
	};

	printk("DHMNv1.56:br_init> Starting DHCP-Me-Not Config Listener: %s\n",__FUNCTION__);
	nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);

	if(!nl_sk){
    		printk(KERN_ALERT "DHMNv1.56:br_init> Error creating socket.\n");
    		return 10;
	}

	return 0;
}

// ************* EXIT AND CLEANUP PROCEDURES (Called by br.c) *****************
void br_read_mat_exit(void){
	printk(KERN_INFO "DHMNv1.56:br_init> Stopping DHCP-Me-Not Config Listener\n");
	netlink_kernel_release(nl_sk);
}
MODULE_LICENSE("GPL");
