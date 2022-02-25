#pragma once
#include <iostream>
#include <cstddef>
#include <chrono>
#include <sys/time.h>
#include <ctime>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

class metronome
{
public:
    enum { beat_samples = 5 };

public:
    metronome()
            : m_timing(false), m_beat_count(0) {}
    ~metronome() {}

public:
    // Call when entering "learn" mode
    void start_timing(){
        m_beat_count=1;
        auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        m_beats[0] = millisec_since_epoch;
        m_timing = true;
    }
    // Call when leaving "learn" mode
    void stop_timing(){
        m_timing = false;
        for(int i = 0;i<m_beat_count;i++){
            m_beats[i]=0;
        }
    }

    // Should only record the current time when timing
    // Insert the time at the next free position of m_beats
    void tap(){
        auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        if(is_timing()){
            if(m_beat_count<5){
                m_beats[m_beat_count] = millisec_since_epoch;
                m_beat_count++;
            }
            else{
                for(int i = 0; i<4; i++){
                    m_beats[i]=m_beats[i+1];
                }
                m_beats[4]=millisec_since_epoch;
            }
        }
    }
    bool is_timing() const { return m_timing; }
    // Calculate the BPM from the deltas between m_beats
    // Return 0 if there are not enough samples
    size_t get_bpm() const{
        cout<<"Got into get_bpm"<<endl;
        size_t bpm = 0;
        if(m_beat_count == 5){
            int points = m_beat_count;
            while(points > 1){
                cout<<m_beats[points-1]<<endl;
                cout<<m_beats[points-2]<<endl;
                bpm += m_beats[points-1] - m_beats[points-2];
                cout<<"In place "<<points<<" we have: "<<m_beats[points-1] - m_beats[points-2]<<endl;
                points--;
            }
            cout<<"After loop"<<endl;
            cout<<"BPM: "<<bpm<<" / "<<m_beat_count<<endl;
            bpm = (bpm)/static_cast<float>(m_beat_count-1);
            cout<<"BPM is: "<<bpm<<endl;
            return bpm;
        }
        else{
            return bpm=1;
        }
    }
    void put_bpm(size_t num){
        if(num < min){
            min = num;
        }
        if(num > max){
            max = num;
        }
    }
    size_t get_min(){
        return min;
    }
    void del_min(){
        min = 0;
    }
    size_t get_max(){
        return max;
    }
    void del_max(){
        max = 0;
    }
private:
    bool m_timing;
    size_t min=1000;
    size_t max=1000;
    // Insert new samples at the end of the array, removing the oldest
    size_t m_beats[beat_samples];
    size_t m_beat_count;
};



