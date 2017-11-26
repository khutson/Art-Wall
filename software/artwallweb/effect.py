import time

class Effect():
    def __init__(self):
        self.cycle_time = 0 #seconds - if 0, effect is disabled
        self.lights = []  #Light objects
        self.cycle_start_time = time.monotonic()
        
    def phase(self):
        p =  (time.monotonic()-self.cycle_start_time)/self.cycle_time
        if p<0: 
            p=0.0
            self.cycle_start_time = time.monotonic()
        if p>=1: 
            p=1.0
            self.cycle_start_time = time.monotonic()
        return p
    
    def update(self):
        """update the colors/status of lights at this point in phase
        must be subclassed """
        
        pass
    
    
if __name__ == '__main__':
    testeffect = Effect()
    testeffect.cycle_time = 5.0
    for s in range(5):
        print("phase={}".format(testeffect.phase()))
        time.sleep(1)
        