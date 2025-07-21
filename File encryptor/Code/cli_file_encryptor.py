#!/usr/bin/env python3
"""
cli_file_encryptor.py

A simple CLI tool to symmetrically encrypt/decrypt files with:
 - AES-CBC (128 or 256 bit)
 - PBKDF2-HMAC-SHA256 key derivation with per-file salt
 - HMAC-SHA256 over (salt || iv || ciphertext)
 - Built-in throughput measurement for benchmarking

Usage:
  # encrypt:
  ./cli_file_encryptor.py encrypt -i plaintext.bin -o secret.bin
  # decrypt:
  ./cli_file_encryptor.py decrypt -i secret.bin  -o recovered.bin
"""

import argparse
import time
import getpass
from Crypto.Cipher import AES
from Crypto.Hash import HMAC, SHA256
from Crypto.Protocol.KDF import PBKDF2
from Crypto.Random import get_random_bytes

MAGIC = b'CLIENC'      # 6 bytes
VERSION = 1            # 1 byte
HMAC_SIZE = 32         # SHA256 tag length

def derive_keys(password: bytes, salt: bytes, key_size_bits: int):
    """
    Derive AES key and HMAC key from password+salt using PBKDF2.
    Returns (enc_key, hmac_key).
    """
    key_len = key_size_bits // 8
    dk = PBKDF2(password, salt,
                dkLen=key_len + HMAC_SIZE,
                count=200_000,
                hmac_hash_module=SHA256)
    return dk[:key_len], dk[key_len:]

def encrypt_file(in_path, out_path, password, key_size_bits):
    start = time.time()
    salt = get_random_bytes(16)
    enc_key, hmac_key = derive_keys(password, salt, key_size_bits)
    iv = get_random_bytes(16)

    # Read plaintext
    with open(in_path, 'rb') as f:
        plaintext = f.read()

    # PKCS7 padding
    pad_len = AES.block_size - (len(plaintext) % AES.block_size)
    plaintext += bytes([pad_len]) * pad_len

    # AES-CBC encrypt
    cipher = AES.new(enc_key, AES.MODE_CBC, iv)
    ciphertext = cipher.encrypt(plaintext)

    # HMAC over (salt || iv || ciphertext)
    h = HMAC.new(hmac_key, digestmod=SHA256)
    h.update(salt + iv + ciphertext)
    tag = h.digest()

    # Write out: MAGIC | VERSION | key_len_byte | salt | iv | ciphertext | tag
    key_len_byte = key_size_bits // 8
    with open(out_path, 'wb') as f:
        f.write(MAGIC)
        f.write(bytes([VERSION]))
        f.write(bytes([key_len_byte]))
        f.write(salt)
        f.write(iv)
        f.write(ciphertext)
        f.write(tag)

    # Throughput report
    duration = time.time() - start
    mb = len(ciphertext) / (1024 * 1024)
    print(f"[+] Encrypted {len(plaintext)} bytes → {len(ciphertext)} ciphertext bytes")
    print(f"    AES-{key_size_bits}, Time: {duration:.3f}s, Throughput: {mb/duration:.2f} MB/s")

def decrypt_file(in_path, out_path, password):
    start = time.time()
    with open(in_path, 'rb') as f:
        data = f.read()

    # Parse header
    if not data.startswith(MAGIC):
        raise ValueError("Unrecognized file format")
    offset = len(MAGIC)
    version = data[offset]; offset += 1
    key_len = data[offset]; offset += 1
    salt = data[offset:offset+16]; offset += 16
    iv   = data[offset:offset+16]; offset += 16
    ciphertext = data[offset:-HMAC_SIZE]
    tag        = data[-HMAC_SIZE:]

    # Derive keys
    enc_key, hmac_key = derive_keys(password, salt, key_len * 8)

    # Verify HMAC
    h = HMAC.new(hmac_key, digestmod=SHA256)
    h.update(salt + iv + ciphertext)
    h.verify(tag)  # will raise if mismatch

    # Decrypt AES-CBC
    cipher = AES.new(enc_key, AES.MODE_CBC, iv)
    padded = cipher.decrypt(ciphertext)
    pad_len = padded[-1]
    plaintext = padded[:-pad_len]

    # Write plaintext
    with open(out_path, 'wb') as f:
        f.write(plaintext)

    duration = time.time() - start
    mb = len(ciphertext) / (1024 * 1024)
    print(f"[+] Decrypted {len(ciphertext)} bytes → {len(plaintext)} plaintext bytes")
    print(f"    Time: {duration:.3f}s, Throughput: {mb/duration:.2f} MB/s")

def main():
    parser = argparse.ArgumentParser(description="CLI File Encryptor/Decryptor")
    sub = parser.add_subparsers(dest='cmd', required=True)

    enc = sub.add_parser('encrypt', help='Encrypt a file')
    enc.add_argument('-i','--input',  required=True, help='Plaintext file path')
    enc.add_argument('-o','--output', required=True, help='Ciphertext output path')
    enc.add_argument('--key-size', type=int, choices=[128,256], default=256,
                     help='AES key size in bits (default: 256)')
    enc.add_argument('-p','--password', help='Password (prompted if omitted)')

    dec = sub.add_parser('decrypt', help='Decrypt a file')
    dec.add_argument('-i','--input',  required=True, help='Ciphertext file path')
    dec.add_argument('-o','--output', required=True, help='Recovered plaintext path')
    dec.add_argument('-p','--password', help='Password (prompted if omitted)')

    args = parser.parse_args()
    pwd = args.password.encode() if args.password else getpass.getpass().encode()

    if args.cmd == 'encrypt':
        encrypt_file(args.input, args.output, pwd, args.key_size)
    else:
        decrypt_file(args.input, args.output, pwd)

if __name__ == '__main__':
    main()