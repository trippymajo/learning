import socket
import sys

###
# Sockets are pretty low-level and in python there are such libs as 
# 1) requests - sync requests to http
# 2) httpx - sync/async requests to http
# 3) aiohttp - async requests client/server
# Python:
# 2) urllib.request to simply use urls directly
# 3) http.client to easy access documents via GET, POST, PUT, DELETE
###

def udp(host_name: str, host_port: int, msg: bytes) -> bytes:
    # Ask DNS about IPv4 and IPv6 if needed
    dns_response = socket.getaddrinfo(
        host_name, host_port,
        socket.AF_UNSPEC,
        socket.SOCK_DGRAM,
        socket.IPPROTO_UDP)

    TIMEOUT_SEC = 3.0 # Timeout on connect, send and recv
    BUFFER_SIZE = 65535

    # going through IPvs provided by DNS
    for address in dns_response:
        af, socktype, protocol, canonname, socket_address = address
        try:
            with socket.socket(af, socktype, protocol) as s:
                s.settimeout(TIMEOUT_SEC)
                s.connect(socket_address)   # UDP! Without shaking hand. Just local to use s.send() & s.recv
                s.send(msg)
                resp = s.recv(BUFFER_SIZE)  # Get response
                return resp
        except (socket.timeout, OSError) as err:
            print(f"Could not send/receive via {address}: {err}")

    raise RuntimeError("Failed to send/receive UDP datagram to any resolved address")


def tcp(host_name: str, host_port: int, msg: bytes):
    # Ask DNS about IPv4 and IPv6 if needed
    dns_response = socket.getaddrinfo(
        host_name, host_port,
        socket.AF_UNSPEC,
        socket.SOCK_STREAM,
        socket.IPPROTO_TCP
    )

    DELIM = b"\r\n\r\n"     # Delim to end reading from server
    MAX_HEADER = 64 * 1024  # Max header size
    RECV_SIZE = 4096        # Max size of response buffer
    PER_RECV_TIMEOUT = 3.0  # Timeout on connect, send and recv
    TOTAL_TIMEOUT = 10.0    # Total timeout

    # going through IPvs provided by DNS
    last_err = None
    for address in dns_response:
        af, socktype, protocol, canonname, socket_address = address
        try:
            with socket.socket(af, socktype, protocol) as s:
                s.settimeout(PER_RECV_TIMEOUT)
                s.connect(socket_address)
                s.sendall(msg)

                # Reading response in chunks, until closing or timeout
                chunks = bytearray()
                while True:
                    try:
                        part = s.recv(65536)
                        if not part: # server closed connection
                            break
                        chunks.extend(part)
                    except socket.timeout:
                        break

                return bytes(chunks)
        except OSError as err:
            last_err = err
            print(f"Could not send/receive via {address}: {err}")

    raise RuntimeError(f"Failed to send/receive TCP to {host_name}:{host_port}: {last_err}")


def main():
    is_connection = bool(sys.argv[1] == "1") # is TCP (1) or UDP (0)
    host_name = sys.argv[2] # "www.example.com"
    host_port = int(sys.argv[3])
    msg = sys.argv[4]
    msg = msg.encode("utf-8")

    result = None
    if is_connection:
        result = tcp(host_name, host_port, msg)
    else:
        result = udp(host_name, host_port, msg)

    if result is not None:
        print(f"Response:\n{result.decode('utf-8',  errors='replace')}")



# Entry Point
if __name__ == "__main__":
    main()