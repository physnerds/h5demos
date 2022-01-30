import os,sys
import matplotlib.pyplot as plt
#import ROOT

curr_dir = os.getcwd()
print(curr_dir)
_file = "dump_morebranch_no_token.txt"
_hdf5_file = "blaa.h5"

f = open(_file,'r')

line_counter=0
index_file_open=0
index_file_close=0
file_open_index = -999
#this number 3 comes from : open("blaa.h5", O_RDWR|O_CREAT, 0644)   = 3
#different ways files/objects are open:
rd_only = "O_RDONLY" #READONLY
readwrite = "O_RDWR" #READWRITE
create = "O_CREAT"   #CREATE

#Get the index numbers for different processes
#THIS FILE ALSO HAS THE TIME STAMP WHICH MAKES THINGS INTERESTING....WE CAN LOOK AT THE I/O RATE FOR EACH SYS CALL
#PROCESSES
""""
for line in f:
    if _hdf5_file in line:
        if "open" in line:
            #print(line_counter,"\n",line)
            index_file_open = line_counter
            new_line = line.split("=")
            print(new_line[0]," ",new_line[1])
            num = new_line[1].replace(" ","")
            num = num.replace("\n","")
            
"""

#list to save the system call indexes when opening files
list_open_lines={"read-only":[],
                 "read-write":[],
                 "create":[]
}
#list to save the I/O rate when doing the I/O
list_open_t_rate={"read-only":[],
                 "read-write":[],
                 "create":[]
}
f.seek(0)
for line in f:
    if "open" in line:
        new_line = line.split("=")
        num = new_line[1].replace(" ","")
        num = num.replace("\n","")
        if "Nosuchfile" in num:
            continue
        #print(new_line[1])
        #need to separate into int_num and the I/O rate
        if "<" not in num and ">" not in num:
            continue
        temp_line = new_line[1].replace("\n","")
        temp_line = temp_line.replace(" ","")

        list_temp_line = list(temp_line)
        s_index = list_temp_line.index("<")
        s_end = list_temp_line.index(">")
        t_rate = "" #we want to save the transfer rate somewhere....
        for i in range(s_index+1,s_end):
            t_rate += list_temp_line[i]
        proc_index = list_temp_line[s_index-1]
        
        try:
            int_num = int(proc_index) #we just want to consider the instances where opening something was successful
            f_t_rate = float(t_rate)
        except:
            continue
        if rd_only in line:
            list_open_lines["read-only"].append(int_num)
            list_open_t_rate["read-only"].append(f_t_rate)
        if readwrite in line:
            list_open_lines["read-write"].append(int_num)
            list_open_t_rate["read-write"].append(f_t_rate)
        if create in line:
            list_open_lines["create"].append(int_num)
            list_open_t_rate["create"].append(f_t_rate)
            
            


#now the categories of different things....
print("Total number of readonly acceses ",len(list_open_lines["read-only"]))
print("Total number of read-write access ",len(list_open_lines["read-write"]))
print("Total number of files created ",len(list_open_lines["create"]))

print("min and max transfer rate for readonly access ",min(list_open_t_rate["read-only"]),max(list_open_t_rate["read-only"]))
print("min and max transfer rate for read-write access ",min(list_open_t_rate["read-write"]),max(list_open_t_rate["read-write"]))

#NOW TO CATEGORIZE THE DIFFERENT KINDS OF ACCESSES
set_readonly = set(list_open_lines["read-only"])
print(set_readonly)

print(len(list_open_t_rate["read-only"]), " length of read-only ")
#I think we can see the distribution of the transfer rate for the read-only


print(max(list_open_t_rate["read-only"])," AND ",min(list_open_t_rate["read-only"]))

fig, axs = plt.subplots(1, sharey=True, tight_layout=True)
plt.yscale("log")
plt.xscale("log")
axs.set_xlabel("Time taken (seconds)")
axs.set_ylabel("Number of Reads")
axs.hist(list_open_t_rate["read-only"], bins='auto')
plt.savefig("open_read_only_all.png")


#now count how many of the readonly access were done:
dict_open_lines = {i:list_open_lines["read-only"].count(i) for i in list_open_lines["read-only"]}
print(dict_open_lines)

#lets look at the time taken to read the HDF objects only
rd_hdf_obj = []
temp_list = list_open_lines["read-only"]

for i in range(0,len(temp_list)):
    if temp_list[i]==3:
        rd_hdf_obj.append(list_open_t_rate["read-only"][i])
        
print(len(rd_hdf_obj))

fig, axs = plt.subplots(1, sharey=True, tight_layout=True)
#plt.yscale("log")
plt.xscale("log")
axs.set_xlabel("Time taken (seconds)")
axs.set_ylabel("Number of Reads")
axs.hist(rd_hdf_obj, bins='auto')
plt.savefig("open_read_only_hdf.png")

print("READWRITE:",list_open_lines["read-write"]," \n ","CREATE: ",list_open_lines["create"])
print("TIME TAKEN TO CREATE FILE ",list_open_t_rate["create"])

#now need to find out how many times the write function is called
f.seek(0)
write_counter=0
list_bytes_write=[]
list_write_proc=[]
for line in f:
    if "write(" in line:
        if "3," in line:
            write_counter+=1
            #I now want to categorize in terms of bytes written.
            if write_counter%50000==0:
                print("reading entry ",write_counter)
                
            new_line = line.split("=")
            #print(new_line[0]," AND ",new_line[1])
            val = len(new_line)-1
            num = new_line[val].replace(" ","")
            num = num.replace("\n","")
            if "<" not in num and ">" not in num:
                continue
            temp_line = new_line[val].replace("\n","")
            temp_line = temp_line.replace(" ","")
            
            list_temp_line = list(temp_line)
            s_index = list_temp_line.index("<")
            e_index = list_temp_line.index(">")
            #print(list_temp_line[s_index-1])
            bytes_written = ""
            for i in range(0,s_index):
                bytes_written += list_temp_line[i]
            #print("bytes write ",bytes_written,len(list_temp_line))
            t_rate = ""
            for i in range(s_index+1,e_index):
                t_rate += list_temp_line[i]
            try:
                int_bytes = int(bytes_written)
                f_w_rate = float(t_rate)
            except:
                #print(new_line[1],line)
                continue
            list_bytes_write.append(int_bytes)
            list_write_proc.append(f_w_rate)
            
            
print("INSTANCES OF WRITE ",write_counter,len(list_bytes_write))

#first find the different process indices
set_write = set(list_bytes_write)
print(set_write)
#count how many times each of these bytes are written:
dict_bytes_write = {}

for val in set_write:
    dict_bytes_write[val]=0

dict_list_bytes = {"4":[],
                   "8":[]
}


for i in range(0,len(list_bytes_write)):
    if list_bytes_write[i]==4:
        dict_list_bytes["4"].append(list_write_proc[i])
    if list_bytes_write[i]==8:
        dict_list_bytes["8"].append(list_write_proc[i])
    dict_bytes_write[list_bytes_write[i]]+=1
  
print("INSTANCES OF BYTE WRITES")
for byte in dict_bytes_write:
    print(byte, dict_bytes_write[byte])

dict_list_bytes = {"4":[],
                   "8":[]
}
counter_4_bytes=0
counter_8_bytes=0
for i in range(0,len(list_bytes_write)):
    if list_bytes_write[i]==4:
        dict_list_bytes["4"].append(list_write_proc[i])
        counter_4_bytes+=1
    if list_bytes_write[i]==8:
        dict_list_bytes["8"].append(list_write_proc[i])
        counter_8_bytes+=1
        
print("Instances of 4 bytes write ",counter_4_bytes)
print("Instances of 8 bytes write ",counter_8_bytes)
 
#print(dict_list_bytes["8"])
print(min(dict_list_bytes["4"]),max(dict_list_bytes["4"]))
fig, axs = plt.subplots(1, sharey=True, tight_layout=True)
plt.yscale("log")
plt.ticklabel_format(axis="x", style="sci", scilimits=(0,0))
#plt.xscale("log")
axs.set_xlabel("Time taken (seconds)")
axs.set_ylabel("Number of Writes (4 bytes)")
axs.hist(dict_list_bytes["4"], bins=5)
plt.savefig("open_write_4bytes.png")


fig, axs = plt.subplots(1, sharey=True, tight_layout=True)
plt.yscale("log")
plt.ticklabel_format(axis="x", style="sci", scilimits=(0,0))
#plt.xscale("log")
axs.set_xlabel("Time taken (seconds)")
axs.set_ylabel("Number of Writes (8 bytes)")
axs.hist(dict_list_bytes["8"], bins=5)
plt.savefig("open_write_8bytes.png")


tot_write_list = dict_list_bytes["8"] + dict_list_bytes["4"]

fig, axs = plt.subplots(1, sharey=True, tight_layout=True)
plt.yscale("log")
plt.ticklabel_format(axis="x", style="sci", scilimits=(0,0))
#plt.xscale("log")
axs.set_xlabel("Time taken (seconds)")
axs.set_ylabel("Number of Writes")
axs.hist(tot_write_list, bins=5)
plt.savefig("open_write_allbytes.png")


