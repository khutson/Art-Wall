
class Effect{
  public:
    unsigned long cycle_time; //milliseconds
    unsigned long start_time; //milliseconds
    Effect();
    update();
};


Effect::Effect(){
  cycle_time = 5000;
  start_time = millis();
}
Effect::update(void){
  float phase = float(millis()-start_time) / cycle_time;
  //insert code here to update the lights for the current phase
}

