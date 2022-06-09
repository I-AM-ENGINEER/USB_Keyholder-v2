import tkinter as tk
from tkinter import *
x = 0

def rrrer():
    global x
    x = listbox.curselection()
    res2.configure( text = "%s" % x)
    
top = tk.Tk()
top.title("артем лох")
top['bg']="#51E1D1"
top.geometry("300x350")

frame = Frame(top)
frame1 = Frame(top)
frame2 = Frame(frame)
frame3 = Frame(frame)
lbl = tk.Label(top, text="My Favourite Fruits",bg="Orange",width="200").pack()
listbox = tk.Listbox(frame2)

str1 = "dfsghbdf"
str2 = "yyydfg"
str3 = "asdfsd"
str4 = "jluhbdf"
listbox.insert(1, str1)
listbox.insert(2, str2)
listbox.insert(3, str3)
listbox.insert(4, str4)


closeButton = tk.Button(top, text="открыть",command=rrrer)
res2 = tk.Label(top, text = "-")
frame.pack()
frame2.pack(side = LEFT)
frame3.pack(side = LEFT, expand=True)
#btn = tk.Button(frame, text="delete",bg="orange" ,command=rrrer )

listbox.pack(side = LEFT)
#btn.pack(side = RIGHT)

frame1.pack(expand=True)
 #fill=BOTH 
closeButton.pack(side=LEFT, padx=5, pady=5)
res2.pack(side=LEFT)
#x = listbox.curselection()
#//print(x)

top.mainloop()




    


