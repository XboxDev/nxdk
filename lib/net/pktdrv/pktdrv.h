#ifndef _Pktdrv_
#define _Pktdrv_

int Pktdrv_Init(void);
void Pktdrv_Quit(void);
int Pktdrv_ReceivePackets(void);
void Pktdrv_SendPacket(unsigned char *buffer,int length);
void Pktdrv_GetEthernetAddr(unsigned char *address);
int Pktdrv_GetQueuedTxPkts(void);

#endif
