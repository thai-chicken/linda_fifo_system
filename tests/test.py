from ast import arg
from multiprocessing.dummy import Process
import time
from subprocess import PIPE, Popen
import os
import argparse


def server(directory_name):
    path = os.path.join(directory_name, "server_out.txt")
    with open(path, "wb+") as file_out:
        proc = Popen("./../bin/server", stdout=file_out)
        

def client1(directory_name):
    path_inp = os.path.join(directory_name, "client_1_inp.txt")
    path_out = os.path.join(directory_name, "client_1_out.txt")
    with open(path_out, "wb+") as file_out:
        with open(path_inp, 'rb') as file:
            proc = Popen("./../bin/client", stdin=file, stdout=PIPE)
            for out in proc.stdout:
                file_out.write(out)


def client2(directory_name):
    path_inp = os.path.join(directory_name, "client_2_inp.txt")
    path_out = os.path.join(directory_name, "client_2_out.txt")
    with open(path_out, "wb+") as file_out:
        with open(path_inp, 'rb') as file:
            proc = Popen("./../bin/client", stdin=file, stdout=PIPE)
            for out in proc.stdout:    
                file_out.write(out)
            

def main(dir_name):
    p_server = Process(target=server, args=(dir_name,))
    p_server.start()
    time.sleep(1)
    p_cl1 = Process(target=client1, args=(dir_name,))
    p_cl1.start()
    p_cl2 = Process(target=client2, args=(dir_name,))
    time.sleep(1)
    p_cl2.start()
    p_server.join(timeout=3.0)
    p_cl1.join()
    p_cl2.join()


def save_to_one(dir_name):
    path_out = os.path.join(dir_name, "test_out.txt")
    serv_out = os.path.join(dir_name, "server_out.txt")
    client1_out = os.path.join(dir_name, "client_1_out.txt")
    client2_out = os.path.join(dir_name, "client_2_out.txt")
    with open(path_out, "w+") as f_out:
        f_out.write("SERVER:\n")
    with open(path_out, "ab") as f_out:
        with open(serv_out, "rb") as f_in:
            for line in f_in:
                f_out.write(line)
    with open(path_out, "a") as f_out:
        f_out.write("\n" + "~"*50)
        f_out.write("\nCLIENT 1:\n")
        f_out.write("~"*50 + "\n")
    with open(path_out, "ab") as f_out:
        with open(client1_out, "rb") as f_in:
            for line in f_in:
                f_out.write(line)
    with open(path_out, "a") as f_out:
        f_out.write("\n" + "~"*50)
        f_out.write("\nCLIENT 2:\n")
        f_out.write("~"*50 + "\n")
    with open(path_out, "ab") as f_out:
        with open(client2_out, "rb") as f_in:
            for line in f_in:
                f_out.write(line)


def remove_other_files(dir_name):
    serv_out = os.path.join(dir_name, "server_out.txt")
    client1_out = os.path.join(dir_name, "client_1_out.txt")
    client2_out = os.path.join(dir_name, "client_2_out.txt")
    os.remove(serv_out)
    os.remove(client1_out)
    os.remove(client2_out)

if __name__=="__main__":
    parser = argparse.ArgumentParser(description="tests parser")
    parser.add_argument('--dir', dest='dir', help="directory of tests", required=True)
    args = parser.parse_args()
    main(args.dir)
    save_to_one(args.dir)
    remove_other_files(args.dir)