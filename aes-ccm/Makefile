CC=gcc
CFLAGS=

NETTLE_DEPS = nettle/nettle/aes.h \
	      nettle/nettle/aes-internal.h \
	      nettle/nettle/ccm.h \
	      nettle/nettle/ctr.h \
	      nettle/nettle/macros.h \
	      nettle/nettle/memops.h \
	      nettle/nettle/memxor.h \
	      nettle/nettle/memxor-internal.h \
	      nettle/nettle/nettle-internal.h \
	      nettle/nettle/nettle-types.h

OBJ = codec/hex.o
OPENSSL_OBJ = openssl/enceeper.o
NETTLE_OBJ = nettle/nettle/aes256-set-encrypt-key.o \
	     nettle/nettle/aes-encrypt.o \
	     nettle/nettle/aes-encrypt-internal.o \
	     nettle/nettle/aes-encrypt-table.o \
	     nettle/nettle/aes-set-key-internal.o \
	     nettle/nettle/ccm-aes256.o \
	     nettle/nettle/ccm.o nettle/nettle/ctr.o \
	     nettle/nettle/memeql-sec.o \
	     nettle/nettle/memxor3.o \
	     nettle/nettle/memxor.o \
	     nettle/enceeper.o

all: openssl nettle

codec/hex.o: codec/hex.c codec/hex.h
	$(CC) -c -o $@ $< $(CFLAGS)

openssl/enceeper.o: openssl/enceeper.c decryptor.h
	$(CC) -c -o $@ $< $(CFLAGS)

nettle/%.o: nettle/%.c $(NETTLE_DEPS) decryptor.h
	$(CC) -c -o $@ $< $(CFLAGS)

openssl: $(OBJ) $(OPENSSL_OBJ) decryptor.c
	$(CC) -o decryptor_$@ $^ $(CFLAGS) -lcrypto

nettle: $(OBJ) $(NETTLE_OBJ) decryptor.c
	$(CC) -o decryptor_$@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f codec/*.o openssl/*.o nettle/*.o nettle/nettle/*.o decryptor_nettle decryptor_openssl
