/* rdisksys.h - definitions for remote disk system pseudo-devices */

#ifndef	Nrds
#define	Nrds		1
#endif

/* Remote disk block size */

#define	RD_BLKSIZ	512

/* Global data for the remote disk server */

#ifndef	RD_SERVER_IP
#define	RD_SERVER_IP	"255.255.255.255"
//#define	RD_SERVER_IP	        "128.10.133.61"
#endif

#ifndef	RD_SERVER_PORT
#define	RD_SERVER_PORT	35071//33124	// Mod by Anirudh Pal (1 Below)
#endif

#ifndef	RD_LOC_PORT
#define	RD_LOC_PORT	35071//33124		/* base port number - minor dev	*/
					/*   number is added to insure	*/
					/*   that each device is unique	*/
#endif

/* Control block for remote disk device */

#define	RD_IDLEN	64		/* Size of a remote disk ID	*/
#define	RD_BUFFS	64		/* Number of disk buffers	*/
#define	RD_STACK	8192*2		/* Stack size for comm. process	*/
#define	RD_PRIO		200		/* Priorty of comm. process	*/

/* Constants for state of the device */

#define	RD_FREE		 0		/* device is available		*/
#define	RD_OPEN		 1		/* device is open (in use)	*/
#define	RD_PEND		 2		/* open is pending		*/

/* Operations for request queue */

#define	RD_OP_READ	1		/* Read operation on req. list	*/
#define	RD_OP_WRITE	2		/* Write operation on req. list	*/
#define	RD_OP_SYNC	3		/* Sync operation on req. list	*/

/* Status values for a buffer */

#define	RD_VALID	0		/* Buffer contains valid data	*/
#define	RD_INVALID	1		/* Buffer does not contain data	*/

/* Definition of a buffer with a header that allows the same node to be	*/
/*  used as a request on the request queue, an item in the cache, or a	*/
/*  node on the free list of buffers					*/

struct	rdbuff	{			/* Request list node		*/
	struct	rdbuff	*rd_next;	/* ptr to next node on a list	*/
	struct	rdbuff	*rd_prev;	/* ptr to prev node on a list	*/
	int32	rd_op;			/* operation - read/write/sync	*/
	int32	rd_refcnt;		/* reference count of processes	*/
					/*   reading the block		*/
	uint32	rd_blknum;		/* block number of this block	*/
	int32	rd_status;		/* is buffer currently valid?	*/
	pid32	rd_pid;			/* process that initiated a	*/
					/*   read request for the block	*/
	char	rd_block[RD_BLKSIZ];	/* space to hold one disk block	*/
};

struct	rdscblk	{
	int32	rd_state;		/* state of device		*/
	char	rd_id[RD_IDLEN];	/* Disk ID currently being used	*/
	int32	rd_seq;			/* next sequence number to use	*/
	/* request queue head and tail */
	struct	rdbuff	*rd_rhnext;	/* head of request queue: next	*/
	struct	rdbuff	*rd_rhprev;	/*  and previous		*/
	struct	rdbuff	*rd_rtnext;	/* tail of request queue: next	*/
	struct	rdbuff	*rd_rtprev;	/*  (null) and previous		*/

	/* cache head and tail */
	struct	rdbuff	*rd_chnext;	/* head of cache: next and	*/
	struct	rdbuff	*rd_chprev;	/*  previous			*/
	struct	rdbuff	*rd_ctnext;	/* tail of cache: next (null)	*/
	struct	rdbuff	*rd_ctprev;	/*  and previous		*/

	/* free list head (singly-linked) */

	struct	rdbuff	*rd_free;	/* ptr to free list		*/

	pid32	rd_comproc;		/* process ID of comm. process	*/
	sid32	rd_availsem;		/* semaphore ID for avail buffs	*/
	sid32	rd_reqsem;		/* semaphore ID for requests	*/
	uint32	rd_ser_ip;		/* server IP address		*/
	uint16	rd_ser_port;		/* server UDP port		*/
	uint16	rd_loc_port;		/* local (client) UPD port	*/
	bool8	rd_registered;		/* has UDP port been registered?*/
};


extern	struct	rdscblk	rdstab[];	/* remote disk control block	*/

/* Definitions of parameters used during server access */

#define	RD_RETRIES	3		/* times to retry sending a msg	*/
#define	RD_TIMEOUT	2000		/* wait two seconds for reply	*/

/* Control functions for a remote file pseudo device */

#define	RDS_CTL_DEL	1		/* Delete (erase) an entire disk*/
#define RDS_CTL_SYNC	2		/* Write all pending blocks	*/

/************************************************************************/
/*	Definition of messages exchanged with the remote disk server	*/
/************************************************************************/
/* Values for the type field in messages */

#define	RD_MSG_RESPONSE	0x0100		/* Bit that indicates response	*/

#define	RD_MSG_RREQ	0x0010		/* Read request and response	*/
#define	RD_MSG_RRES	(RD_MSG_RREQ | RD_MSG_RESPONSE)

#define	RD_MSG_WREQ	0x0020		/* Write request and response	*/
#define	RD_MSG_WRES	(RD_MSG_WREQ | RD_MSG_RESPONSE)

#define	RD_MSG_OREQ	0x0030		/* Open request and response 	*/
#define	RD_MSG_ORES	(RD_MSG_OREQ | RD_MSG_RESPONSE)

#define	RD_MSG_CREQ	0x0040		/* Close request and response	*/
#define	RD_MSG_CRES	(RD_MSG_CREQ | RD_MSG_RESPONSE)

#define	RD_MSG_DREQ	0x0050		/* Delete request and response 	*/
#define	RD_MSG_DRES	(RD_MSG_DREQ | RD_MSG_RESPONSE)

#define	RD_MIN_REQ	RD_MSG_RREQ	/* Minimum request type		*/
#define	RD_MAX_REQ	RD_MSG_DREQ	/* Maximum request type		*/

/* Message header fields present in each message */

#define	RD_MSG_HDR			/* Common message fields	*/\
	uint16	rd_type;		/* message type			*/\
	uint16	rd_status;		/* 0 in req, status in response	*/\
	uint32	rd_seq;			/* message sequence number	*/\
	char	rd_id[RD_IDLEN];	/* null-terminated disk ID	*/

/************************************************************************/
/*				Header					*/
/************************************************************************/
/* The standard header present in all messages with no extra fields */
#pragma pack(2)
struct	rd_msg_hdr {			/* header fields present in each*/
	RD_MSG_HDR			/*   remote file system message	*/
};
#pragma pack()

/************************************************************************/
/*				Read					*/
/************************************************************************/
#pragma pack(2)
struct	rd_msg_rreq	{		/* remote file read request	*/
	RD_MSG_HDR			/* header fields		*/
	uint32	rd_blk;			/* block number to read		*/
};
#pragma pack()

#pragma pack(2)
struct	rd_msg_rres	{		/* remote file read reply	*/
	RD_MSG_HDR			/* header fields		*/
	uint32	rd_blk;			/* block number that was read	*/
	char	rd_data[RD_BLKSIZ];	/* array containing one block	*/
};
#pragma pack()

/************************************************************************/
/*				Write					*/
/************************************************************************/
#pragma pack(2)
struct	rd_msg_wreq	{		/* remote file write request	*/
	RD_MSG_HDR			/* header fields		*/
	uint32	rd_blk;			/* block number to write	*/
	char	rd_data[RD_BLKSIZ];	/* array containing one block	*/
};
#pragma pack()

#pragma pack(2)
struct	rd_msg_wres	{		/* remote file write response	*/
	RD_MSG_HDR			/* header fields		*/
	uint32	rd_blk;			/* block number that was written*/
};
#pragma pack()

/************************************************************************/
/*				Open					*/
/************************************************************************/
#pragma pack(2)
struct	rd_msg_oreq	{		/* remote file open request	*/
	RD_MSG_HDR			/* header fields		*/
};
#pragma pack()

#pragma pack(2)
struct	rd_msg_ores	{		/* remote file open response	*/
	RD_MSG_HDR			/* header fields		*/
};
#pragma pack()

/************************************************************************/
/*				Close					*/
/************************************************************************/
#pragma pack(2)
struct	rd_msg_creq	{		/* remote file close request	*/
	RD_MSG_HDR			/* header fields		*/
};
#pragma pack()

#pragma pack(2)
struct	rd_msg_cres	{		/* remote file close response	*/
	RD_MSG_HDR			/* header fields		*/
};
#pragma pack()

/************************************************************************/
/*				Delete					*/
/************************************************************************/
#pragma pack(2)
struct	rd_msg_dreq	{		/* remote file delete request	*/
	RD_MSG_HDR			/* header fields		*/
};
#pragma pack()

#pragma pack(2)
struct	rd_msg_dres	{		/* remote file delete response	*/
	RD_MSG_HDR			/* header fields		*/
};
#pragma pack()
