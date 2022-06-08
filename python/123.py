import tkinter as tk
import serial

masLogins = []
num = 0

def fun2():
    global num
    num = num - 1
def fun3():
    global num
    num = num + 1
    
def summation():
    #one = float(num1.get())
    ser.write(b'b\n')
    q = ser.readline()
    
    text = str(num) + '\n'
    ser.write(text.encode('utf-8'))
    
    w = ser.readline()
    e = ser.readline()
    r = ser.readline()
    print(w)
    print(e)
    print(r)

    res.configure( text = "%s" % w.decode('utf-8')[0:-1])
    res1.configure(text = "%s" % e.decode('utf-8')[0:-1])
    res2.configure(text = "%s" % r.decode('utf-8')[0:-1])

ser = serial.Serial('COM3', 38400, timeout=1)

win = tk.Tk()
win.title("артем лох")
win.geometry("200x200")

button = tk.Button(win, text = "да", command = summation)
button.grid(row = 0, column = 1)
button1 = tk.Button(win, text = "<", command = fun2)
button1.grid(row = 0, column = 0)
button2 = tk.Button(win, text = ">", command = fun3)
button2.grid(row = 0, column = 2)

#num1 = tk.Entry(win)
#num1.grid(row = 1, column = 0)

res = tk.Label(win, text = "-")
res.grid(row = 3, column = 1)
res1 = tk.Label(win, text = "-")
res1.grid(row = 4, column = 1)
res2 = tk.Label(win, text = "-")
res2.grid(row = 5, column = 1)

win.mainloop()
