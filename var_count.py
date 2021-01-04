from tqdm import tqdm

f = open("fini.cnf", "r")
num_lst = []
count = 0 
for x in tqdm(f):
    if x[0]=='p':
        continue
    for num in x.split():
        num1 = int(num)
        if num1 == 0:
            continue
        if num1<0:
            if abs(num1) not in num_lst:
                if num1 not in num_lst:
                    num_lst.append(num1)
                    count+=1
                    #print(count)
            else:
                print("wrong !", num1)
                exit(0)
        if num1>0:
            
            if -abs(num1) not in num_lst:
                if num1 not in num_lst:
                    num_lst.append(num1)
                    count+=1
                    #print(count)
            else:
                print("wrong !",num1)
                exit(0)
                
print ( "count", count )
