from tkinter import *
x = 0

def rrrer():
    global x
    x = listbox.curselection()
    print(x)

top = Tk()
top['bg']="#51E1D1"
top.geometry("300x250")
lbl = Label(top, text="My Favourite Fruits",bg="Orange",width="200").pack()
listbox = Listbox(top)
listbox.insert(1, "Apple")
listbox.insert(2, "Orange")
listbox.insert(3, "Cherry")
listbox.insert(4, "Mango")
btn = Button(top, text="delete",bg="orange" ,command=rrrer )
listbox.pack(pady=5)
btn.pack()



#x = listbox.curselection()
#//print(x)

top.mainloop()




    


