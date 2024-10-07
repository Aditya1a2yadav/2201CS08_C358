from scapy.all import *

def ping(dest, count=4, pkt_size=64, ttl=64, timeout=2):
    try:
        ip = IP(dst=dest, ttl=ttl)
    except:
        print(f"Failed to resolve '{dest}'.")
        return
    if count <= 0 or pkt_size <= 0 or ttl <= 0:
        print("Parameters 'count', 'pkt_size', and 'ttl' must be positive.")
        return
    for _ in range(count):
        packet = ip / ICMP() / (b'Q' * pkt_size)
        try:
            reply = sr1(packet, timeout=timeout, verbose=0)
            if reply:
                rtt = (reply.time - packet.sent_time) * 1000
                print(f"Response from {reply.src}: size={pkt_size} time={rtt:.2f}ms TTL={reply.ttl}")
            else:
                print("Request timed out.")
        except Exception as e:
            print(f"Error occurred: {e}")
            break

if __name__ == "_main_":
    dest = input("Enter destination: ")
    count = int(input("Enter count: "))
    pkt_size = int(input("Enter packet size: "))
    ttl = int(input("Enter TTL: "))
    timeout = int(input("Enter timeout: "))
    ping(dest, count, pkt_size, ttl, timeout)