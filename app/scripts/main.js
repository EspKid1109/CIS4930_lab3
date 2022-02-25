
const start_new_button = document.querySelector("#start_new").onclick = start_new;
const refresh_current_button = document.querySelector("#refresh_current").onclick = refresh_current;
const refresh_min_button = document.querySelector("#refresh_min").onclick = refresh_min;
const refresh_max_button = document.querySelector("#refresh_max").onclick = refresh_max;
const reset_current_button = document.querySelector("#reset_current").onclick = reset_current;
const reset_min_button = document.querySelector("#reset_min").onclick = reset_min;
const reset_max_button = document.querySelector("#reset_max").onclick = reset_max;

const request = new XMLHttpRequest()

const URI = 'http://localhost:8080/bpm'

document.getElementById('start_new').disabled = true; //Disable at first
const og_test = document.getElementById('newBPM').innerText;

document.getElementById('new_bpm').addEventListener('keyup', e => {
  //Check for the input's value
  if (!e.target.value) {
    document.getElementById('start_new').disabled = true;
    document.getElementById('newBPM').innerText = og_test;
  }
  else {
    document.getElementById('start_new').disabled = false;
    document.getElementById('newBPM').innerText = "";

  }
});

request.addEventListener('readystatechange', () => {
    if(request.readyState === 4){
        var res = JSON.parse(request.responseText)
        add_response(res.mode);
        console.log(res.mode);
        console.log("Done");
    }
})

function start_new(){
    console.log("Start New clicked");
    change_task("Start New");
    var data = document.querySelector("#new_bpm").value;
    console.log("New BPM", data);
    request.open('GET', URI);

    const json = {
        "newBPM": data
    };

    request.send(JSON.stringify(json))
};

function refresh_current(){
    console.log("Refresh Current BPM clicked");
    change_task("Refresh Current BPM");
};

function refresh_max(){
    console.log("Refresh Maximum BPM clicked");
    change_task("Refresh Maximum BPM");
};

function refresh_min(){
    console.log("Refresh Minimum BPM clicked");
    change_task("Refresh Minimum BPM");
};

function reset_current(){
    console.log("Reset Current BPM clicked");
    change_task("Reset Current BPM");
};

function reset_min(){
    console.log("Reset Minimum BPM clicked");
    change_task("Reset Minimum BPM");
};

function reset_max(){
    console.log("Reset Maximum BPM clicked");
    change_task("Reset Maximum BPM");
};

function change_task(task){
    const task_text = document.querySelector("#task_runned");
    const bold = document.createElement("strong")
    bold.innerText = task;
    task_text.innerText = "";
    task_text.append(bold);

};

function add_response(res){
    if (document.querySelector("#response")){
        document.querySelector("#response").remove();
    }
    const res_text = document.createElement("p");
    res_text.id = "response";
    const under = document.createElement("u")
    under.innerText = res;
    res_text.innerText = "";
    res_text.append(under);
    res_text.className = "lead text-center"
    document.body.append(res_text);  
};