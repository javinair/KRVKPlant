import argparse
import asyncio
import telnetlib3

async def telnet_client(host, port, show_console, file_output):
    reader, writer = await telnetlib3.open_connection(host, port)
    
    log_file = None
    if file_output:
        log_file = open(file_output, "w")
    
    async def read_from_server():
        try:
            while True:
                data = await reader.read(1024)
                if not data:
                    break
                
                if show_console:
                    print(data, end="")
                
                if log_file:
                    log_file.write(data)
                    log_file.flush()
        except asyncio.CancelledError:
            pass
        finally:
            if log_file:
                log_file.close()
            writer.close()
            await writer.wait_closed()
    
    async def send_commands():
        while True:
            command = await asyncio.to_thread(input, "")
            writer.write(command + "\n")
            await writer.drain()
            if command.lower() == "q":
                break
    
    tasks = [
        asyncio.create_task(read_from_server()),
        asyncio.create_task(send_commands())
    ]
    
    await asyncio.gather(*tasks, return_exceptions=True)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Telnet client for KRVKPlant")
    parser.add_argument("-c", action="store_true", help="Show output in console")
    parser.add_argument("-f", metavar="file", type=str, help="Save output to file")
    parser.add_argument("--host", type=str, default="krvkplant", help="Telnet server hostname")
    parser.add_argument("--port", type=int, default=23, help="Telnet server port")
    
    args = parser.parse_args()
    
    asyncio.run(telnet_client(args.host, args.port, args.c, args.f))