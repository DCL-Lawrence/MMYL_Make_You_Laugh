# MMYL Make You Laugh
## Introduction
MMYL Make You Laugh is an project to implement MMYL protocol. <br>
MMYL is a HTTP style protocol to get jokes. <br>
You can use my client example to get jokes after running my server example.

## Compile
You can compile client and server example with following
1. `lex -t mmyl/decode.l > mmyl/decode.c`
2. `gcc mmyl/encode.c mmyl/decode.c mmyl/dtype.c server.c -lrt -o server`
3. `gcc mmyl/encode.c mmyl/decode.c mmyl/dtype.c client.c -o client`
<br>
or run
<br>
`make`

## Create Ollama container.
We use Ollama docker image for large language model
1. Create dictionary: `mkdir mmyl/joke/ollama`
2. Install ollama APIs: `pip3 install -r mmyl/joke/re.txt`
3. Install: `docker pull ollama/ollama`
4. Run container: `docker run -d -v ollama:/root/.ollama -p 11434:11434 --name ollama ollama/ollama`
5. Execute phi model: `docker exec -it ollama ollama run phi`
<br>
or run
<br>
`make install`

## Remove Ollama container.
You can use following command
1. Remove ollama dirtionary: `rmdir mmyl/joke/ollama`
2. Stop ollama container: `docker stop ollama`
3. Remove ollama container: `docker rm ollama`
4. Remove ollama image: `docker rmi ollama`
<br>
or run
<br>
`make uninstall`
