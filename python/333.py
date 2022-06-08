import tkinter as tk

x = 0

def rrrer():
    global x
    x = listbox.curselection()
    res2.configure( text = "%s" % x)
top = tk.Tk()
top.title("артем лох")
top['bg']="#51E1D1"
top.geometry("300x250")
lbl = tk.Label(top, text="My Favourite Fruits",bg="Orange",width="200").pack()
listbox = tk.Listbox(top)


str1 = "dfsghbdf"
str2 = "yyydfg"
str3 = "asdfsd"
str4 = "jluhbdf"
listbox.insert(1, str1)
listbox.insert(2, str2)
listbox.insert(3, str3)
listbox.insert(4, str4)

btn = tk.Button(top, text="delete",bg="orange" ,command=rrrer )
res2 = tk.Label(top, text = "-")
listbox.pack(pady=5)
btn.pack()
res2.pack()


#x = listbox.curselection()
#//print(x)

top.mainloop()




    


