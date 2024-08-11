obj := mmyl/encode.c mmyl/decode.c mmyl/dtype.c

all: $(obj)
	gcc $(obj) server.c -lrt -o server 
	gcc $(obj) client.c -o client

install:
	mkdir mmyl/joke/ollama
	pip3 install -r mmyl/joke/re.txt
	docker pull ollama/ollama
	docker run -d -v ollama:/root/.ollama -p 11434:11434 --name ollama ollama/ollama
	docker exec -it ollama ollama run phi

clean:
	rm server
	rm client

uninstall:
	rmdir mmyl/joke/ollama
	docker stop ollama
	docker rm ollama
	docker rmi ollama/ollama
