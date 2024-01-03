import shutil
import sys

if __name__ == '__main__':
        where = sys.argv[1]
        to = sys.argv[2]
        with open(".\\NT5\\FileList.txt","r") as f:
            for file in f.readlines():
                shutil.copy(where+file.split("\n")[0],to)
        shutil.copytree(where+"SYSTEM32",to+"\\SYSTEM32")
        shutil.copytree(where,to+"\\..\\$WIN_NT$.~LS\\I386")
