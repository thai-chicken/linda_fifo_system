from multiprocessing.dummy import Process
import subprocess
import time
from subprocess import PIPE, Popen

def server():
    with open("serv_out.txt", "wb+") as file_out:
        proc = Popen("./../../bin/server", stdout=PIPE)
        for out in proc.stdout:
            file_out.write(out)

def client1():
    with open("client_1_out.txt", "wb+") as file_out:
        with open("client_1_inp.txt", 'rb') as file:
            proc = Popen("./../../bin/client", stdin=PIPE, stdout=PIPE, shell=True)
            for arg in file:
                for out in proc.stdout:
                    file_out.write(out)
                file_out.write(arg)
                proc.stdin.write(arg)


def client2():
    with open("client_2_out.txt", "wb+") as file_out:
        with open("client_2_inp.txt", 'rb') as file:
            proc = Popen("./../../bin/client", stdin=PIPE, stdout=PIPE, shell=True)
            for arg in file:
                for out in proc.stdout:    
                    file_out.write(out)
                file_out.write(arg)
                proc.stdin.write(arg)


def main():
    p_server = Process(target=server)
    p_server.start()
    time.sleep(1)
    p_cl1 = Process(target=client1)
    p_cl1.start()
    p_cl2 = Process(target=client2)
    time.sleep(1)
    p_cl2.start()
    p_cl1.join()
    p_cl2.join()
    p_server.terminate()

if __name__=="__main__":
    main()