Import("env")

def prepareWebapp():
    env.Execute("make prepare")

prepareWebapp()