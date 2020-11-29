f = open("/test/demofile2.txt", "ab", 0)
print("test")
while True:
    try: # oh no, we will hang for ever when we run out of disk space
        f.write(b"Now the file has more content!"*1024)
    except:
        pass
f.close()
print("test2")