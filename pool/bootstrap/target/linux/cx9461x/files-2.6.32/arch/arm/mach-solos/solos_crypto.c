/*
 * linux/arch/arm/mach-solos/solos_crypto.c
 *
 * Author: Guido Barzini
 * Copyright (C) 2005 Conexant inc.
 *
 * Weak key checking taken from crypto/des.c : see that file for Copyrights
 * Most of the MD5 code taken from crypto/md5.c : see that file for Copyrights
 * Most of the SHA1 code taken from crypto/sha1.c : see that file for Copyrights
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */


#include <linux/crypto.h>
#include <linux/dma-mapping.h>
#include <asm/hardware.h>
#include <asm/dma.h>
#include <asm/semaphore.h>
#include <asm/arch/hardware/msc16.h>
#include <asm/arch/msc_drv.h>

#define DES_KEYSIZE  (8)
#define DES3_KEYSIZE (24)
#define DES_BLOCKSIZE (8)

#define MD5_DIGEST_SIZE		(16)
#define MD5_HMAC_BLOCK_SIZE	(64)
#define MD5_BLOCK_WORDS		(16)
#define MD5_HASH_WORDS		(4)

#define SHA1_DIGEST_SIZE	(20)
#define SHA1_HMAC_BLOCK_SIZE	(64)


#define CMD_BITS (6)
#define CIPHER_BITS (5)
#define DATALEN_BITS (11)
#define IVLEN_BITS (2)
#define KEYLEN_BITS (3)

#define CMD_MASK ((1 << CMD_BITS) - 1)
#define CIPHER_MASK ((1 << CIPHER_BITS) - 1)
#define DATALEN_MASK ((1 << DATALEN_BITS) - 1)
#define IVLEN_MASK ((1 << IVLEN_BITS) - 1)
#define KEYLEN_MASK ((1 << KEYLEN_BITS) - 1)

#define MSC16_NULL_CMD   (0)
#define MSC16_AESDES_CMD (1)
#define MSC16_SHA_CMD    (2)
#define MSC16_MD5_CMD    (3)
#define MSC16_RNG_CMD    (4)

/* DES modes */
#define MSC16_DES_ECB  (0)
#define MSC16_3DES_ECB (1)
#define MSC16_DES_CBC  (2)
#define MSC16_3DES_CBC (3)
#define MSC16_DES_OFB  (4)
#define MSC16_3DES_OFB (5)

/* MD5 modes */
#define MSC16_MD5_CONT (0)
#define MSC16_MD5_START (0x10)

#define MSC16_SHA_SHA1 (0)
#define MSC16_SHA_SHA256 (256)

#define MSC_DECRYPT_FLAG (0x10)


/* You'll need to mess with crypto/tcrypt.c to use this: do_test needs to be made 
 * an exported symbol */
//#define RUN_CRYPTO_TEST

#ifdef RUN_CRYPTO_TEST
extern void do_test(void);
#endif


struct solos_des_ctxt {
	u8 iv[DES_BLOCKSIZE];
	u8 key[DES_KEYSIZE];
};

struct solos_3des_ctxt {
	u8 iv[DES_BLOCKSIZE];
	u8 key[DES3_KEYSIZE];
};

struct solos_md5_ctx {
	u32 hash[MD5_HASH_WORDS];
	u32 block[MD5_BLOCK_WORDS];
	u64 byte_count;
};

struct sha1_ctx {
        u64 count;
        u32 state[5];
        u8 buffer[SHA1_HMAC_BLOCK_SIZE];
};

struct solos_hw_cmd {
	u32 cmd;
	union {
		struct {
			u32 key;
			u32 iv;
			u32 src;
			u32 dst;
		} crypt;
		struct {
			u32 start;
			u32 data;
			u32 result;
		} hash;
	};
};

#define SRAM_BUFF_SIZE (256)
static u8 *sram_buffer = 0;

static volatile u8 *cmd_buffer = 0;
/* Buffers as used. */
static u8 *tmp_buffer;
static u8 *iv_buffer;
static u8 *key_buffer;
static u8 *hash_buffer;
static u8* data_buffer;

static int registered_des = 0;
static int registered_3des = 0;
static int registered_md5 = 0;
static int registered_sha1 = 0;

/* None of this is reentrant at the moment */
static struct semaphore hw_sem;


static inline u32 sram2msc16(void *addr)
{
	return ((u32)addr & ~SOLOS_SRAM_VIRT) | SOLOS_SRAM_PHYS;
}


/* We probably need to copy the data anyway since we have no knowledge of 
 * src and target alignments and therefore may not have any safe way of 
 * doing cache coherency on them, so it's probably quickest to use SRAM 
 * for the bounce buffers.*/
static int init_copro(void *dummy)
{
	int rc = msc16_load_fw(MSC_CRYPT_ID);
	if ( rc == 0 ) {
		u32 addr;
		sram_buffer = solos_sram_alloc( sizeof(struct solos_hw_cmd) +
						2*DES_BLOCKSIZE + DES3_KEYSIZE + SHA1_DIGEST_SIZE + SRAM_BUFF_SIZE );
		if ( sram_buffer == NULL ) {
			return -ENOMEM;
		}
		cmd_buffer = sram_buffer;
		tmp_buffer = sram_buffer + sizeof(struct solos_hw_cmd);
		iv_buffer = tmp_buffer + DES_BLOCKSIZE;
		key_buffer = iv_buffer + DES_BLOCKSIZE;
		hash_buffer = key_buffer + DES3_KEYSIZE;
		data_buffer = hash_buffer + SHA1_DIGEST_SIZE; /* Big enough for SHA1 or MD5 */
		addr = sram2msc16(sram_buffer); /* The MSC would like a bus address... */
		msc16_write_periph(MSC_CRYPT_ID, MSC16_ADMA_CTRL, MSC16_ADMA_CTRL_ENABLE);
		printk("Passing command buffer %x to MSC16\n", addr );
		msc16_run_fw(MSC_CRYPT_ID, addr & 0xffff, addr >> 16 );
		up(&hw_sem);
#ifdef RUN_CRYPTO_TEST
	__set_current_state(TASK_INTERRUPTIBLE);
	schedule_timeout(HZ*30);
	printk("Running test...\n");
	do_test();
#endif
	}
	return rc;
}

static u32 make_cmd(u32 cmd, u32 cipher, u32 keysize, u32 ivsize, u32 datasize)
{
	/* convert to the correct units */
	keysize = keysize >> 3;
	ivsize = ivsize >> 3;
	datasize = datasize >> 3;

	return ( (cmd & CMD_MASK) << 21 ) | (( cipher & CIPHER_MASK) << 16) | ((datasize & DATALEN_MASK) << 5) 
		| ((ivsize & IVLEN_MASK) << 3) | (keysize & KEYLEN_MASK); 
}

static const u8 parity[] = {
	8,1,0,8,0,8,8,0,0,8,8,0,8,0,2,8,0,8,8,0,8,0,0,8,8,0,0,8,0,8,8,3,
	0,8,8,0,8,0,0,8,8,0,0,8,0,8,8,0,8,0,0,8,0,8,8,0,0,8,8,0,8,0,0,8,
	0,8,8,0,8,0,0,8,8,0,0,8,0,8,8,0,8,0,0,8,0,8,8,0,0,8,8,0,8,0,0,8,
	8,0,0,8,0,8,8,0,0,8,8,0,8,0,0,8,0,8,8,0,8,0,0,8,8,0,0,8,0,8,8,0,
	0,8,8,0,8,0,0,8,8,0,0,8,0,8,8,0,8,0,0,8,0,8,8,0,0,8,8,0,8,0,0,8,
	8,0,0,8,0,8,8,0,0,8,8,0,8,0,0,8,0,8,8,0,8,0,0,8,8,0,0,8,0,8,8,0,
	8,0,0,8,0,8,8,0,0,8,8,0,8,0,0,8,0,8,8,0,8,0,0,8,8,0,0,8,0,8,8,0,
	4,8,8,0,8,0,0,8,8,0,0,8,0,8,8,0,8,5,0,8,0,8,8,0,0,8,8,0,8,0,6,8,
};

static int weak_key(const u8 *key)
{
	u32 n, w;

	n  = parity[key[0]]; n <<= 4;
	n |= parity[key[1]]; n <<= 4;
	n |= parity[key[2]]; n <<= 4;
	n |= parity[key[3]]; n <<= 4;
	n |= parity[key[4]]; n <<= 4;
	n |= parity[key[5]]; n <<= 4;
	n |= parity[key[6]]; n <<= 4;
	n |= parity[key[7]];
	w = 0x88888888L;
	
 	/* 1 in 10^10 keys passes this test */
 	if( !((n - (w >> 3)) & w) ) {
		if (n < 0x41415151) {
			if (n < 0x31312121) {
				if (n < 0x14141515) {
					/* 01 01 01 01 01 01 01 01 */
					if (n == 0x11111111) goto weak;
					/* 01 1F 01 1F 01 0E 01 0E */
					if (n == 0x13131212) goto weak;
				} else {
					/* 01 E0 01 E0 01 F1 01 F1 */
					if (n == 0x14141515) goto weak;
					/* 01 FE 01 FE 01 FE 01 FE */
					if (n == 0x16161616) goto weak;
				}
			} else {
				if (n < 0x34342525) {
					/* 1F 01 1F 01 0E 01 0E 01 */
					if (n == 0x31312121) goto weak;
					/* 1F 1F 1F 1F 0E 0E 0E 0E (?) */
					if (n == 0x33332222) goto weak;
				} else {
					/* 1F E0 1F E0 0E F1 0E F1 */
					if (n == 0x34342525) goto weak;
					/* 1F FE 1F FE 0E FE 0E FE */
					if (n == 0x36362626) goto weak;
				}
			}
		} else {
			if (n < 0x61616161) {
				if (n < 0x44445555) {
					/* E0 01 E0 01 F1 01 F1 01 */
					if (n == 0x41415151) goto weak;
					/* E0 1F E0 1F F1 0E F1 0E */
					if (n == 0x43435252) goto weak;
				} else {
					/* E0 E0 E0 E0 F1 F1 F1 F1 (?) */
					if (n == 0x44445555) goto weak;
					/* E0 FE E0 FE F1 FE F1 FE */
					if (n == 0x46465656) goto weak;
				}
			} else {
				if (n < 0x64646565) {
					/* FE 01 FE 01 FE 01 FE 01 */
					if (n == 0x61616161) goto weak;
					/* FE 1F FE 1F FE 0E FE 0E */
					if (n == 0x63636262) goto weak;
				} else {
					/* FE E0 FE E0 FE F1 FE F1 */
					if (n == 0x64646565) goto weak;
					/* FE FE FE FE FE FE FE FE */
					if (n == 0x66666666) goto weak;
				}
			}
		}
	}
	return 0;
weak:
	return 1;
}
static inline int check_des_key(const u8 *key, unsigned int keylen, u32 *flags)
{
	if ( keylen != DES_KEYSIZE ) {
		*flags |= CRYPTO_TFM_RES_BAD_KEY_LEN;
		return -EINVAL;
	}
	/* Check for weak keys if asked to. */
	if (*flags & CRYPTO_TFM_REQ_WEAK_KEY) {
		if (weak_key(key) )
			*flags |= CRYPTO_TFM_RES_WEAK_KEY;
			return -EINVAL;
	}
	return 0;
}

static int solos_des_setkey(void *ctx, const u8 *key, unsigned int keylen, u32 *flags)
{
	struct solos_des_ctxt *dc = ctx;
	int rc = check_des_key(key, keylen, flags);
	if (rc == 0)
		memcpy(dc->key, key, keylen);
	return rc;
}

/* Should probably move more common stuff into here. Need to check effect
 * on code generation. */
static inline void crypt_helper(u32 cmdval)
{
	struct solos_hw_cmd *cmd = (struct solos_hw_cmd *)cmd_buffer;	
	cmd->cmd = cmdval;
	cmd->crypt.key = sram2msc16(key_buffer);
	cmd->crypt.iv = sram2msc16(iv_buffer);
	cmd->crypt.src = sram2msc16(tmp_buffer);
	cmd->crypt.dst = cmd->crypt.src;
	msc16_handshake(MSC_CRYPT_ID);
}


static void solos_des_encrypt(void *ctx, u8 *dst, const u8 *src)
{
	struct solos_des_ctxt *dc = ctx;
	down(&hw_sem);
	memcpy(tmp_buffer, src, DES_BLOCKSIZE);
	memcpy(iv_buffer, dc->iv, DES_BLOCKSIZE);
	memcpy(key_buffer, dc->key, DES_KEYSIZE );
	crypt_helper(make_cmd(MSC16_AESDES_CMD, MSC16_DES_ECB, DES_KEYSIZE, DES_BLOCKSIZE, DES_BLOCKSIZE ));
	memcpy(dst, tmp_buffer, DES_BLOCKSIZE);
	up(&hw_sem);
}

static void solos_des_decrypt(void *ctx, u8 *dst, const u8 *src)
{
	struct solos_des_ctxt *dc = ctx;
	down(&hw_sem);
	memcpy(tmp_buffer, src, DES_BLOCKSIZE);
	memcpy(iv_buffer, dc->iv, DES_BLOCKSIZE);
	memcpy(key_buffer, dc->key, DES_KEYSIZE );
	crypt_helper( make_cmd(MSC16_AESDES_CMD, MSC16_DES_ECB | MSC_DECRYPT_FLAG, DES_KEYSIZE, DES_BLOCKSIZE, DES_BLOCKSIZE));
	memcpy(dst, tmp_buffer, DES_BLOCKSIZE);
	up(&hw_sem);
}

static int solos_3des_setkey(void *ctx, const u8 *key, unsigned int keylen, u32 *flags)
{
	struct solos_des_ctxt *dc = ctx;
	/* As explained in des.c, just need to check middle key does't match other 2 */
	if (!(memcmp(key, &key[DES_KEYSIZE], DES_KEYSIZE) && 
	    memcmp(&key[DES_KEYSIZE], &key[DES_KEYSIZE * 2],
	    					DES_KEYSIZE))) {

		*flags |= CRYPTO_TFM_RES_BAD_KEY_SCHED;
		return -EINVAL;
	}
	memcpy(dc->key, key, keylen);
	return 0;
}


static void solos_3des_encrypt(void *ctx, u8 *dst, const u8 *src)
{	
	struct solos_des_ctxt *dc = ctx;
	down(&hw_sem);
	memcpy(tmp_buffer, src, DES_BLOCKSIZE);
	memcpy(iv_buffer, dc->iv, DES_BLOCKSIZE);
	memcpy(key_buffer, dc->key, DES3_KEYSIZE );
	crypt_helper( make_cmd(MSC16_AESDES_CMD, MSC16_3DES_ECB, DES3_KEYSIZE, DES_BLOCKSIZE, DES_BLOCKSIZE ) );
	memcpy(dst, tmp_buffer, DES_BLOCKSIZE);
	up(&hw_sem);
}

static void solos_3des_decrypt(void *ctx, u8 *dst, const u8 *src)
{	
	struct solos_des_ctxt *dc = ctx;
	down(&hw_sem);
	memcpy(tmp_buffer, src, DES_BLOCKSIZE);
	memcpy(iv_buffer, dc->iv, DES_BLOCKSIZE);
	memcpy(key_buffer, dc->key, DES3_KEYSIZE );
	crypt_helper( make_cmd(MSC16_AESDES_CMD, MSC16_3DES_ECB | MSC_DECRYPT_FLAG, DES3_KEYSIZE, DES_BLOCKSIZE, DES_BLOCKSIZE) );
	memcpy(dst, tmp_buffer, DES_BLOCKSIZE);
	up(&hw_sem);
}

static void solos_md5_init(void *ctx)
{
	struct solos_md5_ctx *mctx = ctx;

	mctx->hash[0] = 0x67452301;
	mctx->hash[1] = 0xefcdab89;
	mctx->hash[2] = 0x98badcfe;
	mctx->hash[3] = 0x10325476;
	mctx->byte_count = 0;
}

/* FIXME: there is no need for this to deal in such small blocks, really. The MSC16
 * can do larger chunks. 
 */
static void solos_md5_transform(u32 *hash, u32 *block)
{	
	struct solos_hw_cmd *cmd = (struct solos_hw_cmd *)cmd_buffer;
	cmd->cmd = ((MSC16_MD5_CMD & CMD_MASK) << 21 ) | (MSC16_MD5_CONT << 16) | MD5_HMAC_BLOCK_SIZE;
	memcpy(hash_buffer, hash, MD5_DIGEST_SIZE);
	memcpy(data_buffer, block, MD5_HMAC_BLOCK_SIZE);
	cmd->hash.result = sram2msc16(hash_buffer);
	cmd->hash.start = sram2msc16(hash_buffer);
	cmd->hash.data = sram2msc16(data_buffer);
	msc16_handshake(MSC_CRYPT_ID);
	memcpy(hash, hash_buffer, MD5_DIGEST_SIZE);
}

static void solos_md5_update(void *ctx, const u8 *data, unsigned int len)
{
	struct solos_md5_ctx *mctx = ctx;
	const u32 avail = sizeof(mctx->block) - (mctx->byte_count & 0x3f);
	mctx->byte_count += len;

	if (avail > len) {
		memcpy((char *)mctx->block + (sizeof(mctx->block) - avail),
		       data, len);
		return;
	}

	memcpy((char *)mctx->block + (sizeof(mctx->block) - avail),
	       data, avail);

	/* Lock the hardware */
	down(&hw_sem);

	solos_md5_transform(mctx->hash, mctx->block);
	data += avail;
	len -= avail;

	while (len >= sizeof(mctx->block)) {
		memcpy(mctx->block, data, sizeof(mctx->block));
		solos_md5_transform(mctx->hash, mctx->block);
		data += sizeof(mctx->block);
		len -= sizeof(mctx->block);
	}
	up(&hw_sem);

	memcpy(mctx->block, data, len);
}

static void solos_md5_final(void *ctx, u8 *out)
{
	struct solos_md5_ctx *mctx = ctx;
	const unsigned int offset = mctx->byte_count & 0x3f;
	char *p = (char *)mctx->block + offset;
	int padding = 56 - (offset + 1);
	down(&hw_sem);	
	*p++ = 0x80;
	if (padding < 0) {
		memset(p, 0x00, padding + sizeof (u64));
		solos_md5_transform(mctx->hash, mctx->block);
		p = (char *)mctx->block;
		padding = 56;
	}

	memset(p, 0, padding);
	mctx->block[14] = mctx->byte_count << 3;
	mctx->block[15] = mctx->byte_count >> 29;
	solos_md5_transform(mctx->hash, mctx->block);
	memcpy(out, mctx->hash, sizeof(mctx->hash));
	memset(mctx, 0, sizeof(*mctx));
	up(&hw_sem);
}

static void solos_sha1_init(void *ctx)
{
	struct sha1_ctx *sctx = ctx;
	static const struct sha1_ctx initstate = {
	  0,
	  { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 },
	  { 0, }
	};
	*sctx = initstate;
	sctx->state[0] = be32_to_cpu(initstate.state[0]);
	sctx->state[1] = be32_to_cpu(initstate.state[1]);
	sctx->state[2] = be32_to_cpu(initstate.state[2]);
	sctx->state[3] = be32_to_cpu(initstate.state[3]);
	sctx->state[4] = be32_to_cpu(initstate.state[4]);
}

static void solos_sha1_transform(u32 *hash, u8 *data)
{	
	struct solos_hw_cmd *cmd = (struct solos_hw_cmd *)cmd_buffer;
	cmd->cmd = ((MSC16_SHA_CMD & CMD_MASK) << 21 ) | (MSC16_SHA_SHA1 << 16) | SHA1_HMAC_BLOCK_SIZE;
	memcpy(hash_buffer, hash, SHA1_DIGEST_SIZE);
	memcpy(data_buffer, data, SHA1_HMAC_BLOCK_SIZE);
	cmd->hash.result = sram2msc16(hash_buffer);
	cmd->hash.start = sram2msc16(hash_buffer);
	cmd->hash.data = sram2msc16(data_buffer);
	msc16_handshake(MSC_CRYPT_ID);
	memcpy(hash, hash_buffer, SHA1_DIGEST_SIZE);
}


static void solos_sha1_update(void *ctx, const u8 *data, unsigned int len)
{
	struct sha1_ctx *sctx = ctx;
	unsigned int i, j;

	j = (sctx->count >> 3) & 0x3f;
	sctx->count += len << 3;

	if ((j + len) > 63) {
		memcpy(&sctx->buffer[j], data, (i = 64-j));
		solos_sha1_transform(sctx->state, sctx->buffer);
		for ( ; i + 63 < len; i += 64) {
			solos_sha1_transform(sctx->state, &data[i]);
		}
		j = 0;
	}
	else i = 0;
	memcpy(&sctx->buffer[j], &data[i], len - i);
}


/* Add padding and return the message digest. */
static void solos_sha1_final(void* ctx, u8 *out)
{
	struct sha1_ctx *sctx = ctx;
	u32 i, j, index, padlen;
	u64 t;
	u8 bits[8] = { 0, };
	static const u8 padding[64] = { 0x80, };

	t = sctx->count;
	bits[7] = 0xff & t; t>>=8;
	bits[6] = 0xff & t; t>>=8;
	bits[5] = 0xff & t; t>>=8;
	bits[4] = 0xff & t; t>>=8;
	bits[3] = 0xff & t; t>>=8;
	bits[2] = 0xff & t; t>>=8;
	bits[1] = 0xff & t; t>>=8;
	bits[0] = 0xff & t;

	/* Pad out to 56 mod 64 */
	index = (sctx->count >> 3) & 0x3f;
	padlen = (index < 56) ? (56 - index) : ((64+56) - index);
	solos_sha1_update(sctx, padding, padlen);

	/* Append length */
	solos_sha1_update(sctx, bits, sizeof bits); 

	/* Store state in digest */
	for (i = j = 0; i < 5; i++, j += 4) {
		u32 t2 = cpu_to_be32(sctx->state[i]);
		out[j+3] = t2 & 0xff; t2>>=8;
		out[j+2] = t2 & 0xff; t2>>=8;
		out[j+1] = t2 & 0xff; t2>>=8;
		out[j  ] = t2 & 0xff;
	}

	/* Wipe context */
	memset(sctx, 0, sizeof *sctx);
}

struct crypto_alg solos_des_alg =  {
	.cra_list = LIST_HEAD_INIT(solos_des_alg.cra_list),
	.cra_flags = CRYPTO_ALG_TYPE_CIPHER,
	.cra_blocksize = DES_BLOCKSIZE,
	.cra_ctxsize = sizeof(struct solos_des_ctxt),
	.cra_name = "des",
	.cra_u = { .cipher = {
	.cia_min_keysize = DES_KEYSIZE,
	.cia_max_keysize = DES_KEYSIZE,
	.cia_setkey = solos_des_setkey,
	.cia_encrypt = solos_des_encrypt,
	.cia_decrypt = solos_des_decrypt } },
	.cra_module = THIS_MODULE
};

struct crypto_alg solos_3des_alg =  {
	.cra_list = LIST_HEAD_INIT(solos_3des_alg.cra_list),
	.cra_flags = CRYPTO_ALG_TYPE_CIPHER,
	.cra_blocksize = DES_BLOCKSIZE,
	.cra_ctxsize = sizeof(struct solos_3des_ctxt),
	.cra_name = "des3_ede",
	.cra_u = { .cipher = {
	.cia_min_keysize = DES3_KEYSIZE,
	.cia_max_keysize = DES3_KEYSIZE,
	.cia_setkey = solos_3des_setkey,
	.cia_encrypt = solos_3des_encrypt,
	.cia_decrypt = solos_3des_decrypt } },
	.cra_module = THIS_MODULE
};

static struct crypto_alg solos_md5_alg = {
	.cra_name	=	"md5",
	.cra_flags	=	CRYPTO_ALG_TYPE_DIGEST,
	.cra_blocksize	=	MD5_HMAC_BLOCK_SIZE,
	.cra_ctxsize	=	sizeof(struct solos_md5_ctx),
	.cra_module	=	THIS_MODULE,
	.cra_list	=	LIST_HEAD_INIT(solos_md5_alg.cra_list),
	.cra_u		=	{ .digest = {
	.dia_digestsize	=	MD5_DIGEST_SIZE,
	.dia_init   	= 	solos_md5_init,
	.dia_update 	=	solos_md5_update,
	.dia_final  	=	solos_md5_final } }
};

static struct crypto_alg solos_sha1_alg = {
	.cra_name	=	"sha1",
	.cra_flags	=	CRYPTO_ALG_TYPE_DIGEST,
	.cra_blocksize	=	SHA1_HMAC_BLOCK_SIZE,
	.cra_ctxsize	=	sizeof(struct sha1_ctx),
	.cra_module	=	THIS_MODULE,
	.cra_list       =       LIST_HEAD_INIT(solos_sha1_alg.cra_list),
	.cra_u		=	{ .digest = {
	.dia_digestsize	=	SHA1_DIGEST_SIZE,
	.dia_init   	= 	solos_sha1_init,
	.dia_update 	=	solos_sha1_update,
	.dia_final  	=	solos_sha1_final } }
};
 
static int __init solos_crypt_init(void)
{
	int rc;
	init_MUTEX_LOCKED( &hw_sem );
	rc = kernel_thread(init_copro, NULL, 0);
	if ( rc >= 0 ) {
		rc = crypto_register_alg(&solos_des_alg);
		if ( rc < 0 )
			goto out;
		registered_des = 1;
		rc = crypto_register_alg(&solos_3des_alg);
		if (rc < 0 )
			goto out;
		registered_3des = 1;
		rc = crypto_register_alg(&solos_md5_alg);
		if (rc < 0 ) 
			goto out;
 		registered_md5 = 1;
		rc = crypto_register_alg(&solos_sha1_alg);
		if ( rc < 0 )
			goto out;
		registered_sha1 = 1;
	}
out:
	return rc;
}

static void __exit solos_crypt_fini(void)
{
	msc16_stop(MSC_CRYPT_ID);
	if (registered_des) 
		crypto_unregister_alg(&solos_des_alg);
	if (registered_3des)
		crypto_unregister_alg(&solos_3des_alg);
	if (registered_md5)
		crypto_unregister_alg(&solos_md5_alg);
	if (registered_sha1)
		crypto_unregister_alg(&solos_sha1_alg);
	/* FIXME: use solos_sram_free as soon as it's written. */
}

/* This must be late_initcall for everything it needs to be in place. */
late_initcall(solos_crypt_init); 
module_exit(solos_crypt_fini);
