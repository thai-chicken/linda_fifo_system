from multiprocessing.dummy import Process
import time
from subprocess import PIPE, Popen
import os


def server():
    with open("serv_out.txt", "wb+") as file_out:
        proc = Popen("./../../bin/server", stdout=file_out)
        

def client1():
    with open("client_1_out.txt", "wb+") as file_out:
        with open("client_1_inp.txt", 'rb') as file:
            proc = Popen("./../../bin/client", stdin=file, stdout=PIPE)
            for out in proc.stdout:
                file_out.write(out)


def client2():
    with open("client_2_out.txt", "wb+") as file_out:
        with open("client_2_inp.txt", 'rb') as file:
            proc = Popen("./../../bin/client", stdin=file, stdout=PIPE)
            for out in proc.stdout:    
                file_out.write(out)
            

def main():
    p_server = Process(target=server)
    p_server.start()
    time.sleep(1)
    p_cl1 = Process(target=client1)
    p_cl1.start()
    p_cl2 = Process(target=client2)
    time.sleep(1)
    p_cl2.start()
    p_server.join(timeout=3.0)
    p_cl1.join()
    p_cl2.join()


def save_to_one():
    with open("test_out.txt", "w+") as f_out:
        f_out.write("SERVER:\n")
    with open("test_out.txt", "ab") as f_out:
        with open("serv_out.txt", "rb") as f_in:
            for line in f_in:
                f_out.write(line)
    with open("test_out.txt", "a") as f_out:
        f_out.write("\n" + "~"*50)
        f_out.write("\nCLIENT 1:\n")
        f_out.write("~"*50 + "\n")
    with open("test_out.txt", "ab") as f_out:
        with open("client_1_out.txt", "rb") as f_in:
            for line in f_in:
                f_out.write(line)
    with open("test_out.txt", "a") as f_out:
        f_out.write("\n" + "~"*50)
        f_out.write("\nCLIENT 2:\n")
        f_out.write("~"*50 + "\n")
    with open("test_out.txt", "ab") as f_out:
        with open("client_2_out.txt", "rb") as f_in:
            for line in f_in:
                f_out.write(line)


def remove_other_files():
    os.remove("serv_out.txt")
    os.remove("client_1_out.txt")
    os.remove("client_2_out.txt")

if __name__=="__main__":
    main()
    save_to_one()
    remove_other_files()