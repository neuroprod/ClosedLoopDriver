#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "cinder/params/Params.h"
#include "cinder/Serial.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class ClosedLoopStepperApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    void writeIntData(int command,unsigned int data);
    
    params::InterfaceGlRef	mParams;
    Serial serial;
    float rotation;
    int maxSpeed;
    float Kp;
    
    float maxAccel;
};

void ClosedLoopStepperApp::setup()
{
    rotation=0;
    maxSpeed =2000;
   
    Kp=20;
    maxAccel=200;
    //connect serial
    const vector<Serial::Device> &devices( Serial::getDevices() );
	for( vector<Serial::Device>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt ) {
		console() << "Device: " << deviceIt->getName() << endl;
	}
	
	try {
		Serial::Device dev = Serial::findDeviceByNameContains("tty.usbmodem1451");
		serial = Serial( dev, 115200);
        console() << "Serial Connected " << endl;
	}
	catch( ... ) {
		console() << "There was an error initializing the serial device!" << std::endl;
		
	}
    
    //
    
    mParams = params::InterfaceGl::create( getWindow(), "MotorParams ", toPixels( Vec2i( 200, 400 ) ) );
    mParams->addParam( "rotation", &rotation).min( -90.0f ).max( 90.0f ).updateFn( [this] { writeIntData(1,(rotation+90)*100); } );
    mParams->addParam( "maxSpeed", &maxSpeed).min( 100.0f ).max( 4000.0f ).updateFn( [this] { writeIntData(2,maxSpeed) ;} );
    mParams->addParam( "Kp", &Kp).step(0.05).min( 1.0f ).max( 300.0f ).updateFn( [this] { writeIntData(4,Kp*10); } );
    mParams->addParam( "maxAccel", &maxAccel).step(0.05).min( 1.0f ).max( 1000.0f ).updateFn( [this] { writeIntData(5,maxAccel*10); } );
    
 
   
}
void ClosedLoopStepperApp::writeIntData(int command, unsigned int data)
{
    int i0 =data/10000;
    data-= i0*10000;
    int i1 =data/100;
    int i2 =data -i1*100;
    serial.writeByte(command);
    serial.writeByte(i0);
    serial.writeByte(i1);
    serial.writeByte(i2);
    serial.writeByte(0xFF);

}
void ClosedLoopStepperApp::mouseDown( MouseEvent event )
{
    if(event.isControlDown())
    {
        if( rotation==-90)
        {
            rotation =90;
        }else
        {
        
            rotation =-90;
        }
    }
    else
    {
    rotation=rand()%180 -90;
    }
    writeIntData(1,(rotation+90)*100);
}

void ClosedLoopStepperApp::update()
{
}

void ClosedLoopStepperApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    mParams->draw();
}

CINDER_APP_NATIVE( ClosedLoopStepperApp, RendererGl )
