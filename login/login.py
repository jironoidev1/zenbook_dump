import tkinter
from tkinter import *

def login():
    username = "Pak28"
    password = "12345"

    if username_entry.get().lower() == username.lower() and password_entry.get().lower() == password.lower() :
        monkey_frame.tkraise()
        print("fufu")
    else:
        print(username_entry.get().lower(), password_entry.get().lower())

window = tkinter.Tk()

login_frame = Frame(window)
monkey_frame = Frame(window)

login_lb = tkinter.Label(login_frame, text="Login", bg= "black", fg="cyan", font=("Aria",32))
username_lb = tkinter.Label(login_frame, text="UserName", bg="black", fg="cyan")
username_entry = tkinter.Entry(login_frame)
password_lb = tkinter.Label(login_frame, text="password", bg="black", fg="cyan")
password_entry = tkinter.Entry(login_frame, show="#")
login_Butt = tkinter.Button(login_frame, text= "login", bg="black", fg="cyan", command= login)

monkey_lb = tkinter.Label(monkey_frame, text="monkey", bg= "black", fg="cyan", font=("Aria",32))


login_lb.grid(row=0,column=1)
username_lb.grid(row=1, column=0)
username_entry.grid(row=1, column=1)
password_lb.grid(row=2, column=0)
password_entry.grid(row=2,column=1)
login_Butt.grid(row=3, column=1)

monkey_lb.pack()

login_frame.grid(row=0, column=0, sticky=NSEW)
monkey_frame.grid(row=0, column=0, sticky=NSEW)

login_frame.config(bg="black")
monkey_frame.config(bg="black")

login_frame.tkraise()
window.title("Pak's Login page")
window.geometry("320x240")
window.resizable(True, True)
window.config(bg="black")

window.mainloop()