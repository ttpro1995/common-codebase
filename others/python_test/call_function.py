
def test1():
    print 'Function: test1'

def test2():
    print 'Function: test2'

ss = {
        'test1' : test1, 
        'test2' : test2 
     }

ss['test1']()

