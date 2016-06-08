#!/usr/bin/python3

import socket, sys
import tkinter as tk


class Configuration:
    """Front-end to file based configuration"""
    def __init__(self, file):
        self.filename = file
        self.config = {}
        with open(self.filename, 'r') as file:
            for line in file:
                self.config[line.split()[0]] = line.split()[1]

    def save(self):
        """Saves configuration to file with truncate"""
        with open(self.filename, 'w') as file:
            file.truncate()
            for k in self.config:
                file.write(k + " " + self.config[k] + "\n")

    def get(self, key):
        return self.config[key]

    def set(self, key, value):
        self.config[key] = value

class Connection(socket.socket):
    """Manages low level connection"""
    def connect(self, host, port):
        super(Connection, self).connect((host, port))

    def disconnect(self):
        super(Connection, self).close()

class Server(Connection):
    """Manages connection to server and connection's configurations at once"""
    def __init__(self, config):
        self.config = Configuration(config)
        Connection.__init__(self, socket.AF_INET, socket.SOCK_STREAM)
        self.connect(self.config.get("Host"), int(self.config.get("Port")))

class IRCServer(Server):
    """Manages connection with server of type IRC"""
    BUFF_SIZE = 4096
    
    def __init__(self, config):
        Server.__init__(self, config)
        self.cmd('NICK', (self.config.get('Nick'),))
        self.recv(self.BUFF_SIZE)
        self.on_recv(self.recv(self.BUFF_SIZE))
        self.on_recv(self.recv(self.BUFF_SIZE))
        self.cmd('USER', (
            self.config.get('User'),
            '- -',
            ':' + self.config.get('Realname')
        ))
        self.on_recv(self.recv(self.BUFF_SIZE))
        self.on_recv(self.recv(self.BUFF_SIZE))
        self.join(self.config.get('Channel'))
        self.start_loop()
        
    def send(self, msg):
        print('sending: %s' % msg)
        super(IRCServer, self).send(bytes(msg + "\r\n", 'utf-8'))
        
    def recv(self, buff_size):
        s = str(super(IRCServer, self).recv(self.BUFF_SIZE), 'utf-8')
        print('received: %s' % s)
        return s
        
    def start_loop(self):
        self.loop = True
        while self.loop:
            self.on_recv(self.recv(self.BUFF_SIZE))

    def exit_loop(self):
        self.loop = False

    def on_recv(self, msg):
        msgs = [i for i in msg.split("\r\n") if i is not '']
        for msg in msgs:
            msg = msg.split()
            if msg[0] == 'PING':
                self.ping(msg[1])
            elif len(msg) > 4 and msg[3] == ':.':
                if msg[4] == 'hello':
                    self.privmsg(msg[2], "Welcome on %s channel! :D" % msg[2])
                elif msg[4] == 'who_are_you?':
                    self.privmsg(msg[2], "I'm an integral part of Wired. Nice to meet you :)")
                elif msg[4] == 'exit':
                    self.exit_loop()

    def cmd(self, command, args):
        m = [command]
        m.extend(args)
        print(m)
        self.send(' '.join(m))
        
    def ping(self, pl):
        self.cmd('PONG', (pl,))

    def join(self, channel):
        self.cmd('JOIN', (channel,))

    def part(self, channel):
        self.cmd('PART', (channel,))
        
    def privmsg(self, target, msg):
        self.cmd('PRIVMSG', (target, ':' + msg))

class GUI:
    def __init__(self, root, config):
        self.config = Configuration(config)

        self.realnameL = tk.Label(root, text="Realname")
        self.realnameL.grid(columnspan=2)
        self.realname = tk.Entry(root, width=100)
        self.realname.grid(row=2, column=1)
        self.realname.insert(0, self.config.get('Realname'))

        self.nickL = tk.Label(root, text="Nick")
        self.nickL.grid(columnspan=2)
        self.nick = tk.Entry(root, width=100)
        self.nick.grid(columnspan=2, sticky=tk.S+tk.W)
        self.nick.insert(0, self.config.get('Nick'))

        self.hostL = tk.Label(root, text="Host")
        self.hostL.grid(columnspan=2)
        self.host = tk.Entry(root, width=100)
        self.host.grid(columnspan=2, sticky=tk.S+tk.W)
        self.host.insert(0, self.config.get('Host'))

        self.userL = tk.Label(root, text="User")
        self.userL.grid(columnspan=2)
        self.user = tk.Entry(root, width=100)
        self.user.grid(columnspan=2, sticky=tk.S+tk.W)
        self.user.insert(0, self.config.get('User'))

        self.portL = tk.Label(root, text="Port")
        self.portL.grid(columnspan=2)
        self.port = tk.Entry(root, width=100)
        self.port.grid(columnspan=2, sticky=tk.S+tk.W)
        self.port.insert(0, self.config.get('Port'))

        self.channelL = tk.Label(root, text="Channel")
        self.channelL.grid(columnspan=2)
        self.channel = tk.Entry(root, width=100)
        self.channel.grid(columnspan=2, sticky=tk.S+tk.W)
        self.channel.insert(0, self.config.get('Channel'))

        self.btn = tk.Button(root, text="Apply", command=self.save)
        self.btn.grid(columnspan=2, sticky=tk.S)

    def save(self):
        self.config.set('Realname', self.realname.get())
        self.config.set('Nick', self.nick.get())
        self.config.set('Host', self.host.get())
        self.config.set('User', self.user.get())
        self.config.set('Port', self.port.get())
        self.config.set('Channel', self.channel.get())
        self.config.save()
     
if __name__ == '__main__':
    if len(sys.argv) > 1 and sys.argv[1] == 'setting':
        root = tk.Tk()
        root.wm_title('Settings manager')
        app = GUI(root, 'config')
        root.mainloop()
    else:
        IRCServer('config')
