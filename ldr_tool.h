#define LDRPin (A0)

int gv_LDRValue = 0; 

int get_LDR_val() {

  gv_LDRValue = analogRead(LDRPin); 
  
  return gv_LDRValue;

}
