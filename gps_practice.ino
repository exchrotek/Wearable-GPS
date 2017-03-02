//http://aprs.gids.nl/nmea/#rmc

#include <SoftwareSerial.h>
#include <String.h>
#include <Adafruit_NeoPixel.h>
#include <stdlib.h>

// trying to create a text file...

//const char test[] = "test";
//const char mode[] = "w";
//
// char * teste = test[0];
//
//file *fopen(test[],mode[]);



String output = "";
String longitude,latitude = "";
int sampling_rate = 5000; //in microseconds

const int home_long = -118.457929;
const int home_lat = 34.071760; 


const int rxPin = 10;
const int txPin = 11;

//const int north = 1;
//const int south = 2;
//const int west = 3;
//const int east = 4;

//const int green = 3; //north
//const int blue = 5; //south
//const int transparent = 6; //west

SoftwareSerial GPS (rxPin, txPin);

 //Adafruit_NeoPixel n = Adafruit_NeoPixel(1, north, NEO_GRB + NEO_KHZ800);
 //Adafruit_NeoPixel s = Adafruit_NeoPixel(1, south, NEO_GRB + NEO_KHZ800);
 //Adafruit_NeoPixel w = Adafruit_NeoPixel(1, west, NEO_GRB + NEO_KHZ800);
 //Adafruit_NeoPixel e = Adafruit_NeoPixel(1, east, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel dir = Adafruit_NeoPixel(4, 12, NEO_GRB + NEO_KHZ800);

const int num_commas_expected = 11;//according to NMEA protocol, the string should only have 11 commas
String nmea[num_commas_expected+1]; //12 items between commas

void setup() {
  // put your setup code here, to run once:
  GPS.begin(9600);
  Serial.begin(57600);
  
  //pinMode(green,OUTPUT); 
  //pinMode(blue,OUTPUT);
  //pinMode(transparent,OUTPUT);

 /*n.begin();
 s.begin();
 w.begin();
 e.begin();


 n.show();
 s.show();
 w.show();
 e.show();

 n.setBrightness(34);
 s.setBrightness(34);
 w.setBrightness(34);
 e.setBrightness(34);*/
  dir.begin();
  dir.show();
  dir.setBrightness(34);

	

}

void loop() {
	//do{
	  Serial.flush();
	  GPS.flush();
	  // put your main code here, to run repeatedly:
 
  
  

    
	  while(GPS.available() > 0)
	  {
		 GPS.read();
		
   
	  }

  

	  if(GPS.find("$GPRMC,"))
	  {
    
    
		output = GPS.readStringUntil('\n'); //convert each line of the GPS serial output to a string
	  }

	  Serial.println("This is the output of the GPS: " + output+"\nlength:"+output.length());
  
	  int * commaPlacement = new int[output.length()];  

	  int commaCount = 0;
	  for(int j = 0; j < (output.length() - 1); j++)
	  {
	
		  if(output[j] == ',')
		  {
		  
			  commaPlacement[commaCount] = j; //an commaPlacement of values with the placement of where the commmas are in the output string
			  commaCount++; //increments counter
		  }



	  }

  

	  Serial.println("num commas: " + commaCount);

  
	  if(num_commas_expected != commaCount) //if commas in the string is not 11
	  {
		  Serial.println("Error! Not the number of commas expected!" + commaCount);
		  return;
	  }

	  //if(output.substring(commaPlacement[0],commaPlacement[1]) == "A" )
	  {
  
		  nmea[0] = output.substring(0, commaPlacement[0]); 
		  for(int i = 0; i < num_commas_expected; i++)
		  {
			  int place_of_comma = commaPlacement[i];
			  int place_of_next_comma = commaPlacement[i+1];
			  nmea[i+1] = output.substring(place_of_comma + 1 , place_of_next_comma);
			  //fill nmea string array with the data from output, with each element being data between commas
		  }

		  nmea[2] = ConvertLat(); //doing the necessary conversions to latitude
		  nmea[4] = ConvertLng(); //doing the necessary conversions to longitude 
		  latitude =  nmea[2];
		  longitude = nmea[4];


		  

      const char * lats = latitude.c_str(); //converts string into char array
      const char * longs = longitude.c_str();

      int latitude_int = atoi(lats); //converts from char pointer into integer
      int longitude_int = atoi(longs);

      Serial.print("int lat: " + latitude_int);
	  Serial.print("int lon: " + longitude_int);

		  if(latitude_int < home_lat) // go north
		  {
			  dir.setPixelColor(0,100,0,0);
			  //s.setPixelColor(0, 0,0,0); //turn off south LED
			  //n.setPixelColor(0, 100,0,0); // turn on north LED
			 // analogWrite(green,100);
			 // analogWrite(blue,0);
			  
			  Serial.println("go North!\n");
			  delay(500);
		  }

		  else if(latitude_int > home_lat) // go south 
		  {
			  dir.setPixelColor(2,100,0,0);
			/*  n.setPixelColor(0, 0,0,0);
			  s.setPixelColor(0, 100,0,0);*/
			//  analogWrite(green,0);
			  //analogWrite(blue,100);
			  Serial.println("go South!\n");
		  }

		  if(longitude_int < home_long) //go east
		  {
			  dir.setPixelColor(1,100,0,0);
			 /* w.setPixelColor(0, 0,0,0);
			  e.setPixelColor(0, 100,0,0);*/
			 // analogWrite(transparent,0);
			  Serial.println("go East!\n");
			  
		  }

		  else if(longitude_int > home_long) //go west
		  {
			  dir.setPixelColor(3,100,0,0);
			  //e.setPixelColor(0, 0,0,0);
		/*	  w.setPixelColor(0, 100,0,0);*/
			//  analogWrite(transparent,10);
			  Serial.println("go West!\n");
		  }

	/*	  n.show();
		  s.show();
		  w.show();
		  e.show();*/
		  dir.show();


		  // Serial.println("nmea string: ");//print all the nmea data
		  //for(int j = 0; j < (num_commas_expected + 1 - 1); j++)
		  //{
	 
			 // Serial.println(nmea[j]);
		  //}
  


		  //Serial.print("End of nmea data.\n");
	  
		  

		 
	
		  Serial.println("Longitude: " + longitude + "\nLatitude: " + latitude +"\n");

		  delay(sampling_rate);
	  }

	  //else
	  //{
		 // //do nothing and loop over again
	  //}
	  delete [] commaPlacement; //deallocate memory
	  
		//} while(nmea[1] == "A");
	}


String ConvertLat() 
  {
  String posneg = "";
  if (nmea[3] == "S") {
    posneg = "-";
  }
  
  String latfirst;
  float latsecond;
  for (int i = 0; i < nmea[2].length(); i++) 
  {
    if (nmea[2].substring(i, i + 1) == ".") 
    {
      latfirst = nmea[2].substring(0, i - 2);
      latsecond = nmea[2].substring(i - 2).toFloat();
    }
  }
  latsecond = latsecond / 60;
  String CalcLat = "";

  char charVal[9];
  dtostrf(latsecond, 4, 6, charVal);
  for (int i = 0; i < sizeof(charVal); i++)
  {
    CalcLat += charVal[i];
  }
  latfirst += CalcLat.substring(1);
  latfirst = posneg += latfirst;
  return latfirst;
}

String ConvertLng() 
  {
  String posneg = "";
  if (nmea[5] == "W") {
    posneg = "-";
  }

  String lngfirst;
  float lngsecond;
  for (int i = 0; i < nmea[4].length(); i++) {
    if (nmea[4].substring(i, i + 1) == ".") {
      lngfirst = nmea[4].substring(0, i - 2);
      //Serial.println(lngfirst);
      lngsecond = nmea[4].substring(i - 2).toFloat();
      //Serial.println(lngsecond);

    }
  }
  lngsecond = lngsecond / 60;
  String CalcLng = "";
  char charVal[9];
  dtostrf(lngsecond, 4, 6, charVal);
  for (int i = 0; i < sizeof(charVal); i++)
  {
    CalcLng += charVal[i];
  }
  lngfirst += CalcLng.substring(1);
  lngfirst = posneg += lngfirst;
  return lngfirst;
}



