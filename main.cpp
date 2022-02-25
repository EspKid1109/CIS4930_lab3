#include <cstddef>
#include <chrono>
#include <thread>
#include <iostream>
using namespace std::chrono_literals;
using namespace std;
#include <cpprest/http_msg.h>
#include <wiringPi.h>

#include "metronome.hpp"
#include "rest.hpp"

// ** Remember to update these numbers to your personal setup. **
#define LED_RED   27
#define LED_GREEN 22
#define BTN_MODE  23
#define BTN_TAP   24

// Mark as volatile to ensure it works multi-threaded.
volatile bool btn_mode_pressed = false;
volatile size_t bpm = 0;
volatile size_t tbpm=0;
volatile size_t currentMIN=0;
volatile size_t currentMAX=0;

// Run an additional loop separate from the main one.
void blink() {
    bool on = true;
    // ** This loop manages LED blinking. **
    while (true) {
        // The LED state will toggle once every second.
        std::this_thread::sleep_for(1s);

        // Perform the blink if we are pressed,
        // otherwise just set it to off.
        if (btn_mode_pressed)
            on = !on;
        else
            on = false;

        // HIGH/LOW probably equal 1/0, but be explicit anyways.
        //digitalWrite(LED_RED, (on) ? HIGH : LOW);
    }
}

void doTheThing(){
    while(true){
        if(bpm != 0){
            digitalWrite(LED_GREEN,HIGH);
            std::this_thread::sleep_for(std::chrono::milliseconds(bpm));
            digitalWrite(LED_GREEN,LOW);
            std::this_thread::sleep_for(std::chrono::milliseconds(bpm));
        }
    }
}

// This is called when a GET request is made to "/answer".
//void get42(web::http::http_request msg) {
//      msg.reply(200, web::json::value::number(42));
//}

void getbpm(web::http::http_request msg){
    web::http::http_response response(200);
    response.headers().add("Access-Control-Allow-Origin", "*");
    response.headers().add("Access-Control-Allow-Methods", "GET, PUT, DELETE");
    response.headers().add("Access-Control-Allow-Headers", "Content-Type");
    response.set_body(web::json::value::number(bpm));
    msg.reply(response);
    cout<<"Get bpm called"<<endl;
}
void putbpm(web::http::http_request msg){
    web::http::http_response response(200);
    response.headers().add("Access-Control-Allow-Origin", "*");
    response.headers().add("Access-Control-Allow-Methods", "GET, PUT, DELETE");
    response.headers().add("Access-Control-Allow-Headers", "Content-Type");
    response.set_body(web::json::value::number(bpm));
    msg.reply(response);
    cout<<"Put bpm called"<<endl;
}
void getmin(web::http::http_request msg){
    web::http::http_response response(200);
    response.headers().add("Access-Control-Allow-Origin", "*");
    response.headers().add("Access-Control-Allow-Methods", "GET, PUT, DELETE");
    response.headers().add("Access-Control-Allow-Headers", "Content-Type");
    response.set_body(web::json::value::number(currentMIN));
    msg.reply(response);
    cout<<"Get min called"<<endl;
}
void delmin(web::http::http_request msg){
    web::http::http_response response(200);
    response.headers().add("Access-Control-Allow-Origin", "*");
    response.headers().add("Access-Control-Allow-Methods", "GET, PUT, DELETE");
    response.headers().add("Access-Control-Allow-Headers", "Content-Type");
    response.set_body(web::json::value::number(currentMIN));
    msg.reply(response);
    currentMIN = 0;
    cout<<"Del min called"<<endl;
}
void getmax(web::http::http_request msg){
    web::http::http_response response(200);
    response.headers().add("Access-Control-Allow-Origin", "*");
    response.headers().add("Access-Control-Allow-Methods", "GET, PUT, DELETE");
    response.headers().add("Access-Control-Allow-Headers", "Content-Type");
    response.set_body(web::json::value::number(currentMAX));
    msg.reply(response);
    cout<<"Get max called"<<endl;
}
void delmax(web::http::http_request msg){
    web::http::http_response response(200);
    response.headers().add("Access-Control-Allow-Origin", "*");
    response.headers().add("Access-Control-Allow-Methods", "GET, PUT, DELETE");
    response.headers().add("Access-Control-Allow-Headers", "Content-Type");
    response.set_body(web::json::value::number(currentMAX));
    msg.reply(response);
    currentMAX = 0;
    cout<<"Del max called"<<endl;
}
// This program shows an example of input/output with GPIO, along with
// a dummy REST service.
// ** You will have to replace this with your metronome logic, but the
//    structure of this program is very relevant to what you need. **
int main() {
    cout<<"Got to main"<<endl;
    // WiringPi must be initialized at the very start.
    // This setup method uses the Broadcom pin numbers. These are the
    // larger numbers like 17, 24, etc, not the 0-16 virtual ones.
    wiringPiSetupGpio();

    // Set up the directions of the pins.
    // Be careful here, an input pin set as an output could burn out.
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(BTN_MODE, INPUT);
    pinMode(BTN_TAP, INPUT);
    // Note that you can also set a pull-down here for the button,
    // if you do not want to use the physical resistor.
    //pullUpDnControl(BTN_MODE, PUD_DOWN);
    metronome met;
    // Configure the rest services after setting up the pins,
    // but before we start using them.
    // ** You should replace these with the BPM endpoints. **
    auto get_bpm = rest::make_endpoint("/bpm");
    get_bpm.support(web::http::methods::GET, getbpm);

    auto put_bpm  = rest::make_endpoint("/bpm");
    put_bpm.support(web::http::methods::PUT, putbpm);

    auto get_min = rest::make_endpoint("/bpm/min");
    get_min.support(web::http::methods::GET, getmin);

    auto del_min = rest::make_endpoint("/bpm/min");
    del_min.support(web::http::methods::DEL, delmin);

    auto get_max = rest::make_endpoint("/bpm/max");
    get_max.support(web::http::methods::GET, getmax);

    auto del_max = rest::make_endpoint("/bpm/max");
    del_max.support(web::http::methods::DEL, delmax);
    // Start the endpoints in sequence.
    get_bpm.open().wait();
    put_bpm.open().wait();
    get_min.open().wait();
//      del_min.open().wait();
    get_max.open().wait();
//      del_max.open().wait();
    // Use a separate thread for the blinking.
    // This way we do not have to worry about any delays
    // caused by polling the button state / etc.
    std::thread blink_thread(blink);
    blink_thread.detach();

    std::thread metronome(doTheThing);
    metronome.detach();
    cout<<"We got to this point, all good here!"<<endl;


// ** This loop manages reading button state. **
    while (true) {
        // We are using a pull-down, so pressed = HIGH.
        // If you used a pull-up, compare with LOW instead.
        if(digitalRead(BTN_MODE) == HIGH){
            std::this_thread::sleep_for(200ms);
            cout<<"Mode was tapped!"<<endl;
            if(met.is_timing()){
                cout<<"Stop Timing"<<endl;
                tbpm = met.get_bpm();
                met.stop_timing();
                if(tbpm != 1){
                    bpm = tbpm;
                }
                cout<<"Stop isn't a problem"<<endl;
            }
            else{
                cout<<"Start timing"<<endl;
                met.start_timing();
                cout<<"Start isnt a problem"<<endl;
            }
        }

        if(digitalRead(BTN_TAP) == HIGH){
            std::this_thread::sleep_for(200ms);
            cout<<"Tap was tapped!"<<endl;
            if(met.is_timing()){
                met.tap();
            }
        }
        // Delay a little bit so we do not poll too heavily.
        std::this_thread::sleep_for(10ms);

        // ** Note that the above is for testing when the button
        // is held down. For detecting "taps", or momentary
        // pushes, you need to check for a "rising edge" -
        // this is when the button changes from LOW to HIGH... **
    }
    return 0;
}