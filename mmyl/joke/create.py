#! /usr/bin/python3

import sys
from multiprocessing import shared_memory

import ollama

question = f"Give me a {sys.argv[1]} joke in 30 words."

response = ollama.generate(
    model = "phi",
    prompt = f"""<|user|>{question}<|end|>"""
)

shm = shared_memory.SharedMemory(name = sys.argv[2])
buf = shm.buf

for i in range(len(response["response"])):
    buf[i] = ord(response["response"][i])

shm.unlink()
sys.exit(0)
