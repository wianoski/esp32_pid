#include "webserver.h"


void route_home(WebServer& s){
  s.client.println("<!doctype html>");
  s.client.println("<html lang=\"en\">");
  s.client.println("<head>");
  s.client.println("<meta charset=\"utf-8\">");
  s.client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">");
  s.client.println("<title>PID Web Access</title>");
  s.client.println("<!-- Bootstrap core CSS -->");
  s.client.println("<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css\" integrity=\"sha384-JcKb8q3iqJ61gNV9KGb8thSsNjpSL0n8PARn9HuZOnIxN0hoP+VmmDGMN5t9UJ0Z\" crossorigin=\"anonymous\">");
  s.client.println("</head>");
  s.client.println("<body>");
  s.client.println("<style>");
  s.client.println("body {");
  s.client.println("padding-top: 5rem;");
  s.client.println("}");
  s.client.println(".starter-template {");
  s.client.println("}");
  s.client.println(".text-center {text-align: center;}");
  s.client.println(".gauge-chart{");
  s.client.println("width:100%;");
  s.client.println("}");
  s.client.println(".gauge-label{");
  s.client.println("margin-top: -35px;");
  s.client.println("font-size: 20px;");
  s.client.println("font-weight: bolder;");
  s.client.println("}");
  s.client.println(".gauge-subtext{");
  s.client.println("margin-top: -20px;");
  s.client.println("color: #9f9f9f;");
  s.client.println("text-align: center;");
  s.client.println("}");
  s.client.println("#sensor-error{");
  s.client.println("color: red;");
  s.client.println("}");
  s.client.println("</style>");
  s.client.println("<nav class=\"navbar navbar-expand-md navbar-dark bg-dark fixed-top\">");
  s.client.println("<a class=\"navbar-brand\" href=\"#\">PID Controller</a>");
  s.client.println("<button class=\"navbar-toggler\" type=\"button\" data-toggle=\"collapse\" data-target=\"#navbarsExampleDefault\" aria-controls=\"navbarsExampleDefault\" aria-expanded=\"false\" aria-label=\"Toggle navigation\">");
  s.client.println("<span class=\"navbar-toggler-icon\"></span>");
  s.client.println("</button>");
  s.client.println("<div class=\"collapse navbar-collapse\" id=\"navbarsExampleDefault\">");
  s.client.println("<ul class=\"navbar-nav mr-auto\">");
  s.client.println("<li class=\"nav-item active\">");
  s.client.println("<a class=\"nav-link\" href=\"#\">Home <span class=\"sr-only\">(current)</span></a>");
  s.client.println("</li>");
  s.client.println("</ul>");
  s.client.println("</div>");
  s.client.println("</nav>");
  s.client.println("<main role=\"main\" class=\"container\">");
  s.client.println("<div class=\"starter-template\">");
  s.client.println("<div class=\"row\">");
  s.client.println("<div class=\"col-6\">");
  s.client.println("<h1 class=\"text-center\">Input</h1>");
  s.client.println("</div>");
  s.client.println("<div class=\"col-6\">");
  s.client.println("<h1 class=\"text-center\">Output</h1>");
  s.client.println("</div>");
  s.client.println("</div>");
  s.client.println("<div class=\"row\">");
  s.client.println("<div class=\"col-1\">");
  s.client.println("</div>");
  s.client.println("<div class=\"col-4\">");
  s.client.println("<canvas id=\"setpoint-gauge-chart\" class=\"gauge-chart\"></canvas>");
  s.client.println("<p class=\"text-center gauge-label\"><span id=\"input\"></span></p>");
  s.client.println("<p class=\"gauge-subtext\" id=\"sensor-error\" style=\"display: none;\">SENSOR ERROR</p>");
  s.client.println("<p class=\"gauge-subtext\" id=\"redundant-sensor\" style=\"display: none;\">Inputs<span id=\"input-sensors-use-average\"> (a)</span> <span id=\"sensor1-value\"></span> / <span id=\"sensor2-value\"></span></p>");
  s.client.println("<p class=\"gauge-subtext\">Setpoint <span id=\"setpoint\"></span></p>");
  s.client.println("</div>");
  s.client.println("<div class=\"col-2\">");
  s.client.println("</div>");
  s.client.println("<div class=\"col-4\">");
  s.client.println("<canvas id=\"output-gauge-chart\" class=\"gauge-chart\"></canvas>");
  s.client.println("<p class=\"text-center gauge-label\"><span id=\"output\"></span>%<span id=\"outputMode\"></span></p>");
  s.client.println("<p id=\"pid-internal-values\" class=\"gauge-subtext\">(<span id=\"PidOutputP\"></span>/<span id=\"PidOutputI\"></span>/<span id=\"PidOutputD\"></span>) <span id=\"pid-in-use\"></span></p>");
  s.client.println("</div>");
  s.client.println("<div class=\"col-1\">");
  s.client.println("</div>");
  s.client.println("</div>");
  s.client.println("<div class=\"row\" style=\"height:400px;\">");
  s.client.println("<canvas id=\"myChart\" style=\"background-color:#eee;\"></canvas>");
  s.client.println("</div>");
  s.client.println("<!--         <h2>Settings</h2>");
  s.client.println("<form>");
  s.client.println("<h3>Pid 1</h3>");
  s.client.println("<div class=\"form-row\">");
  s.client.println("<div class=\"form-group col-4\">");
  s.client.println("<label for=\"Kp\">Kp</label>");
  s.client.println("<input type=\"text\" class=\"form-control\" id=\"Kp\">");
  s.client.println("</div>");
  s.client.println("<div class=\"form-group col-4\">");
  s.client.println("<label for=\"Ki\">Ki</label>");
  s.client.println("<input type=\"text\" class=\"form-control\" id=\"Ki\">");
  s.client.println("</div>");
  s.client.println("<div class=\"form-group col-4\">");
  s.client.println("<label for=\"Kd\">Kd</label>");
  s.client.println("<input type=\"text\" class=\"form-control\" id=\"Kd\">");
  s.client.println("</div>");
  s.client.println("</div>");
  s.client.println("<h3>Pid 2</h3>");
  s.client.println("<div class=\"form-row\">");
  s.client.println("<div class=\"form-group col-4\">");
  s.client.println("<label for=\"Kp2\">Kp</label>");
  s.client.println("<input type=\"text\" class=\"form-control\" id=\"Kp2\">");
  s.client.println("</div>");
  s.client.println("<div class=\"form-group col-4\">");
  s.client.println("<label for=\"Ki2\">Ki</label>");
  s.client.println("<input type=\"text\" class=\"form-control\" id=\"Ki2\">");
  s.client.println("</div>");
  s.client.println("<div class=\"form-group col-4\">");
  s.client.println("<label for=\"Kd2\">Kd</label>");
  s.client.println("<input type=\"text\" class=\"form-control\" id=\"Kd2\">");
  s.client.println("</div>");
  s.client.println("</div>");
  s.client.println("<div class=\"form-row\">");
  s.client.println("<div class=\"form-group col-4\">");
  s.client.println("<label for=\"pid2Band\">Band</label>");
  s.client.println("<input type=\"text\" class=\"form-control\" id=\"pid2Band\">");
  s.client.println("</div>");
  s.client.println("</div>");
  s.client.println("</form>");
  s.client.println("<p>PidDirection: <span id=\"PidDirection\"></span></p>");
  s.client.println("<p>OperatingMode: <span id=\"OperatingMode\"></span></p>");
  s.client.println("<p>SampleTime: <span id=\"SampleTime\"></span></p>");
  s.client.println("<p>dataLogDelay: <span id=\"dataLogDelay\"></span></p>");
  s.client.println("<p>SettingAdjustmentMultiple: <span id=\"SettingAdjustmentMultiple\"></span></p> -->");
  s.client.println("</div>");
  s.client.println("</main><!-- /.container -->");
  s.client.println("<!-- Bootstrap core JavaScript");
  s.client.println("================================================== -->");
  s.client.println("<!-- Placed at the end of the document so the pages load faster -->");
  s.client.println("<script src=\"https://code.jquery.com/jquery-3.5.1.min.js\" integrity=\"sha256-9/aliU8dGd2tb6OSsuzixeV4y/faTqgFtohetphbbj0=\" crossorigin=\"anonymous\"></script>");
  s.client.println("<script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.bundle.min.js\" integrity=\"sha384-LtrjvnR4Twt/qOuYxE721u19sVFLVSA4hf/rRt6PrZTmiPltdZcI7q7PXQBYTKyf\" crossorigin=\"anonymous\"></script>");
  s.client.println("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.js\" integrity=\"sha512-hZf9Qhp3rlDJBvAKvmiG+goaaKRZA6LKUO35oK6EsM0/kjPK32Yw7URqrq3Q+Nvbbt8Usss+IekL7CRn83dYmw==\" crossorigin=\"anonymous\"></script>");
  s.client.println("<script src=\"https://bernii.github.io/gauge.js/dist/gauge.min.js\" crossorigin=\"anonymous\"></script>");
  s.client.println("<script>");
  s.client.println("var MAX_CHART_LEN = 200;");
  s.client.println("var API_URL = \"\";");
  s.client.println("var GAUGE_SP;");
  s.client.println("var GAUGE_OUTPUT;");
  s.client.println("var INPUT_LAST_MIN;");
  s.client.println("var INPUT_LAST_MAX;");
  s.client.println("function updateGauges(setpoint, input, output){");
  s.client.println("// GAUGE_OUTPUT");
  s.client.println("var diff = Math.abs(input - setpoint);");
  s.client.println("var pad = Math.ceil(diff * 1.1);");
  s.client.println("var min = setpoint - pad;");
  s.client.println("var max = setpoint + pad;");
  s.client.println("if(INPUT_LAST_MIN != min || INPUT_LAST_MAX != max){");
  s.client.println("GAUGE_SP.animationSpeed = 1;");
  s.client.println("}");
  s.client.println("else{");
  s.client.println("GAUGE_SP.animationSpeed = 32; // set animation speed (32 is default value)");
  s.client.println("}");
  s.client.println("if(GAUGE_SP.maxValue != max){");
  s.client.println("GAUGE_SP.maxValue = max; // set max gauge value");
  s.client.println("}");
  s.client.println("//");
  s.client.println("if(GAUGE_SP.minValue != min){");
  s.client.println("GAUGE_SP.setMinValue(min);");
  s.client.println("}");
  s.client.println("//   // Prefer setter over GAUGE_SP.minValue = 0");
  s.client.println("GAUGE_SP.set(input); // set actual value");
  s.client.println("GAUGE_SP.options.staticLabels.labels = [min,max,setpoint];");
  s.client.println("GAUGE_SP.options.renderTicks.divisions = max-min;");
  s.client.println("GAUGE_OUTPUT.set(output); // set actual value");
  s.client.println("GAUGE_OUTPUT.options.staticLabels.labels = [output];");
  s.client.println("INPUT_LAST_MIN = min;");
  s.client.println("INPUT_LAST_MAX = max;");
  s.client.println("}");
  s.client.println("function updateVariables(){");
  s.client.println("$.getJSON( API_URL + \"/variables\", function( data ) {");
  s.client.println("var items = [];");
  s.client.println("console.log(data);");
  s.client.println("$(\"#input\").text(data.In.val);");
  s.client.println("$(\"#setpoint\").text(data.SP);");
  s.client.println("// $(\"#PidDirection\").text(data.Settings.PidDirection);");
  s.client.println("if(data.Md){");
  s.client.println("updateGauges(data.SP,data.In.val,data.O);");
  s.client.println("$(\"#outputMode\").text(\"\");");
  s.client.println("$(\"#output\").text(data.O);");
  s.client.println("$(\"#PidOutputP\").text(data.P);");
  s.client.println("$(\"#PidOutputI\").text(data.I);");
  s.client.println("$(\"#PidOutputD\").text(data.D);");
  s.client.println("if(data.Pr){");
  s.client.println("$(\"#pid-in-use\").text(\"Normal\");");
  s.client.println("}");
  s.client.println("else{");
  s.client.println("$(\"#pid-in-use\").text(\"Agressive\");");
  s.client.println("}");
  s.client.println("$(\"#pid-status\").show();");
  s.client.println("$(\"#pid-internal-values\").show();");
  s.client.println("}");
  s.client.println("else{");
  s.client.println("$(\"#outputMode\").text(\" (Manual)\");");
  s.client.println("$(\"#pid-status\").hide();");
  s.client.println("$(\"#pid-internal-values\").hide();");
  s.client.println("$(\"#output\").text(data.Mn);");
  s.client.println("updateGauges(data.SP,data.In.val,data.Mn);");
  s.client.println("}");
  s.client.println("if(data.In.r){");
  s.client.println("$(\"#redundant-sensor\").show();");
  s.client.println("if(data.In.i1){");
  s.client.println("$(\"#sensor1-value\").text(data.In.i1);");
  s.client.println("}");
  s.client.println("else{");
  s.client.println("$(\"#sensor1-value\").text(\"Err\");");
  s.client.println("}");
  s.client.println("if(data.In.i2){");
  s.client.println("$(\"#sensor2-value\").text(data.In.i2);");
  s.client.println("}");
  s.client.println("else{");
  s.client.println("$(\"#sensor2-value\").text(\"Err\");");
  s.client.println("}");
  s.client.println("}");
  s.client.println("if(data.In.e){");
  s.client.println("$(\"#sensor-error\").show();");
  s.client.println("}else{");
  s.client.println("$(\"#sensor-error\").hide();");
  s.client.println("}");
  s.client.println("if(data.In.a){");
  s.client.println("$(\"#input-sensors-use-average\").show();");
  s.client.println("}else{");
  s.client.println("$(\"#input-sensors-use-average\").hide();");
  s.client.println("}");
  s.client.println("// $(\"#Kp\").val(data.Settings.Kp);");
  s.client.println("// $(\"#Ki\").val(data.Settings.Ki);");
  s.client.println("// $(\"#Kd\").val(data.Settings.Kd);");
  s.client.println("// $(\"#Kp2\").val(data.Settings.Kp2);");
  s.client.println("// $(\"#Ki2\").val(data.Settings.Ki2);");
  s.client.println("// $(\"#Kd2\").val(data.Settings.Kd2);");
  s.client.println("// $(\"#pid2Band\").val(data.Settings.pid2Band);");
  s.client.println("// $(\"#SampleTime\").text(data.Settings.SampleTime);");
  s.client.println("// $(\"#dataLogDelay\").text(data.Settings.dataLogDelay);");
  s.client.println("// $(\"#SettingAdjustmentMultiple\").text(data.Settings.SettingAdjustmentMultiple);");
  s.client.println("});");
  s.client.println("}");
  s.client.println("$( document ).ready(function() {");
  s.client.println("console.log(\"Ready\");");
  s.client.println("updateVariables();");
  s.client.println("var ctx = document.getElementById('myChart').getContext('2d');");
  s.client.println("setInterval(updateVariables, 3000);");
  s.client.println("var opts = {");
  s.client.println("angle: -0.2, // The span of the gauge arc");
  s.client.println("lineWidth: 0.2, // The line thickness");
  s.client.println("radiusScale: 0.85, // Relative radius");
  s.client.println("pointer: {");
  s.client.println("length: 0.6, // // Relative to gauge radius");
  s.client.println("strokeWidth: 0.042, // The thickness");
  s.client.println("color: '#000000' // Fill color");
  s.client.println("},");
  s.client.println("limitMax: false,     // If false, max value increases automatically if value > maxValue");
  s.client.println("limitMin: false,     // If true, the min value of the gauge will be fixed");
  s.client.println("colorStart: '#6F6EA0',   // Colors");
  s.client.println("colorStop: '#C0C0DB',    // just experiment with them");
  s.client.println("strokeColor: '#EEEEEE',  // to see which ones work best for you");
  s.client.println("generateGradient: true,");
  s.client.println("highDpiSupport: true,     // High resolution support");
  s.client.println("// renderTicks is Optional");
  s.client.println("renderTicks: {");
  s.client.println("divisions: 0,");
  s.client.println("divWidth: 0.1,");
  s.client.println("divLength: 0.7,");
  s.client.println("divColor: '#333333',");
  s.client.println("subDivisions: 4,");
  s.client.println("subLength: 0.2,");
  s.client.println("subWidth: 0.6,");
  s.client.println("subColor: '#666666'");
  s.client.println("},");
  s.client.println("staticLabels: {");
  s.client.println("font: \"16px sans-serif\",  // Specifies font");
  s.client.println("labels: [],  // Print labels at these values");
  s.client.println("color: \"#000000\",  // Optional: Label text color");
  s.client.println("fractionDigits: 0  // Optional: Numerical precision. 0=round off.");
  s.client.println("}");
  s.client.println("};");
  s.client.println("var target = document.getElementById('setpoint-gauge-chart'); // your canvas element");
  s.client.println("GAUGE_SP = new Gauge(target).setOptions(opts); // create sexy gauge!");
  s.client.println("var opts = {");
  s.client.println("angle: -0.2, // The span of the gauge arc");
  s.client.println("lineWidth: 0.2, // The line thickness");
  s.client.println("radiusScale: 0.85, // Relative radius");
  s.client.println("pointer: {");
  s.client.println("length: 0.6, // // Relative to gauge radius");
  s.client.println("strokeWidth: 0.042, // The thickness");
  s.client.println("color: '#000000' // Fill color");
  s.client.println("},");
  s.client.println("limitMax: false,     // If false, max value increases automatically if value > maxValue");
  s.client.println("limitMin: false,     // If true, the min value of the gauge will be fixed");
  s.client.println("colorStart: '#6F6EA0',   // Colors");
  s.client.println("colorStop: '#C0C0DB',    // just experiment with them");
  s.client.println("strokeColor: '#EEEEEE',  // to see which ones work best for you");
  s.client.println("generateGradient: true,");
  s.client.println("highDpiSupport: true,     // High resolution support");
  s.client.println("// renderTicks is Optional");
  s.client.println("renderTicks: {");
  s.client.println("divisions: 10,");
  s.client.println("divWidth: 0.1,");
  s.client.println("divLength: 0.7,");
  s.client.println("divColor: '#333333',");
  s.client.println("subDivisions: 2,");
  s.client.println("subLength: 0.2,");
  s.client.println("subWidth: 0.6,");
  s.client.println("subColor: '#666666'");
  s.client.println("},");
  s.client.println("staticLabels: {");
  s.client.println("font: \"16px sans-serif\",  // Specifies font");
  s.client.println("labels: [],  // Print labels at these values");
  s.client.println("color: \"#000000\",  // Optional: Label text color");
  s.client.println("fractionDigits: 0  // Optional: Numerical precision. 0=round off.");
  s.client.println("}");
  s.client.println("};");
  s.client.println("var target = document.getElementById('output-gauge-chart'); // your canvas element");
  s.client.println("GAUGE_OUTPUT = new Gauge(target).setOptions(opts); // create sexy gauge!");
  s.client.println("GAUGE_OUTPUT.maxValue = 100; // set max gauge value");
  s.client.println("GAUGE_OUTPUT.setMinValue(0);  // Prefer setter over GAUGE_OUTPUT.minValue = 0");
  s.client.println("GAUGE_OUTPUT.animationSpeed = 32; // set animation speed (32 is default value)");
  s.client.println("$.getJSON( API_URL + \"/chart_data\", function( data ) {");
  s.client.println("var chart = new Chart(ctx, {");
  s.client.println("// The type of chart we want to create");
  s.client.println("type: 'line',");
  s.client.println("// The data for our dataset");
  s.client.println("data: {");
  s.client.println("labels: Array.from(Array(data.outputData.length).keys()),");
  s.client.println("datasets: [");
  s.client.println("{");
  s.client.println("label: 'Input',");
  s.client.println("backgroundColor: 'rgba(0, 0, 0, 0)',");
  s.client.println("borderColor: 'rgb(255, 0, 0)',");
  s.client.println("data: data.inputData,");
  s.client.println("yAxisID: 'B',");
  s.client.println("},");
  s.client.println("{");
  s.client.println("label: 'Setpoint',");
  s.client.println("backgroundColor: 'rgba(0, 0, 0, 0)',");
  s.client.println("borderColor: 'rgb(255, 155, 0)',");
  s.client.println("data: data.setpointData,");
  s.client.println("yAxisID: 'B',");
  s.client.println("},");
  s.client.println("{");
  s.client.println("label: 'Output',");
  s.client.println("backgroundColor: 'rgba(0, 0, 0, 0)',");
  s.client.println("borderColor: 'rgb(0, 255, 0)',");
  s.client.println("data: data.outputData,");
  s.client.println("yAxisID: 'A',");
  s.client.println("}");
  s.client.println("]");
  s.client.println("},");
  s.client.println("// Configuration options go here");
  s.client.println("options: {");
  s.client.println("scales: {");
  s.client.println("xAxes: [{display:false}],");
  s.client.println("yAxes: [{");
  s.client.println("id: 'A',");
  s.client.println("type: 'linear',");
  s.client.println("position: 'left',");
  s.client.println("ticks: {");
  s.client.println("min: 0,");
  s.client.println("max: 100");
  s.client.println("}");
  s.client.println("}, {");
  s.client.println("id: 'B',");
  s.client.println("type: 'linear',");
  s.client.println("position: 'right'");
  s.client.println("}]");
  s.client.println("},");
  s.client.println("maintainAspectRatio:false,");
  s.client.println("responsive:true");
  s.client.println("}");
  s.client.println("});");
  s.client.println("function addData() {");
  s.client.println("console.log(\"adding data\");");
  s.client.println("$.getJSON( API_URL + \"/last_chart_data\", function( data ) {");
  s.client.println("console.log(\"adding data cb\");");
  s.client.println("l = chart.data.labels.length;");
  s.client.println("chart.data.labels.push(chart.data.labels[l-1]+1);");
  s.client.println("chart.data.datasets[0].data.push(data.inputData);");
  s.client.println("chart.data.datasets[1].data.push(data.setpointData);");
  s.client.println("chart.data.datasets[2].data.push(data.outputData);");
  s.client.println("if(l >MAX_CHART_LEN){");
  s.client.println("chart.data.labels.shift();");
  s.client.println("chart.data.datasets[0].data.shift();");
  s.client.println("chart.data.datasets[1].data.shift();");
  s.client.println("chart.data.datasets[2].data.shift();");
  s.client.println("}");
  s.client.println("chart.update();");
  s.client.println("});");
  s.client.println("}");
  s.client.println("setInterval(addData, 10000);");
  s.client.println("});");
  s.client.println("});");
  s.client.println("</script>");
  s.client.println("</body>");
  s.client.println("</html>");





}


void chart_data(WebServer& s){
  
  double inputData[DATA_ARRAY_SIZE];
  double outputData[DATA_ARRAY_SIZE];
  double setpointData[DATA_ARRAY_SIZE];

  for(int i = 0; i < DATA_ARRAY_SIZE; i++) {
      inputData[i] = s._inputLog->getItem(i);
    }
  
  for(int i = 0; i < DATA_ARRAY_SIZE; i++) {
      outputData[i] = s._outputLog->getItem(i);
    }
  
  for(int i = 0; i < DATA_ARRAY_SIZE; i++) {
      setpointData[i] = s._setpointLog->getItem(i);
    }



  s.client.println("{");
  s.client.println(" \"inputData\": [");
  for(int i = 0; i < DATA_ARRAY_SIZE; i++) {
      if(inputData[i]!=EMPTY_DATA){
        s.client.println(String(inputData[i]));
        if(i!=DATA_ARRAY_SIZE-1){ s.client.println(",");}
      }
  }
  s.client.println("],");

  s.client.println(" \"outputData\": [");
  for(int i = 0; i < DATA_ARRAY_SIZE; i++) {
      if(outputData[i]!=EMPTY_DATA){
        s.client.println(String(outputData[i]));
        if(i!=DATA_ARRAY_SIZE-1){ s.client.println(",");}
      }
  }
  s.client.println("],");

  s.client.println(" \"setpointData\": [");
  for(int i = 0; i < DATA_ARRAY_SIZE; i++) {
      if(setpointData[i]!=EMPTY_DATA){
        s.client.println(String(setpointData[i]));
        if(i!=DATA_ARRAY_SIZE-1){ s.client.println(",");}
      }
  }
  s.client.println("]");
  s.client.println("}");
}

void last_chart_data(WebServer& s){
  s.client.println("{");
  s.client.println(" \"inputData\": " + String(s._inputLog->getItem(DATA_ARRAY_SIZE-1)) +",");
  s.client.println(" \"outputData\": " + String(s._outputLog->getItem(DATA_ARRAY_SIZE-1)) +",");
  s.client.println(" \"setpointData\": " + String(s._setpointLog->getItem(DATA_ARRAY_SIZE-1)));
  s.client.println("}");
}


void route_test(WebServer& s){
  s.client.println("Hi");

}

void variables_get(WebServer& s){
  s.client.println("{");
  s.client.println("\"In\" : {");
  if(!isnan(s.inputState->value)){
    s.client.println("\"val\" : " + String(s.inputState->value) + ",");
  }
  else{
    s.client.println("\"val\" : null,");
  }
  if(!isnan(s.inputState->input1)){
    s.client.println("\"i1\" : " + String(s.inputState->input1) + ",");
  }
  else{
    s.client.println("\"i1\" : null,");
  }
  if(!isnan(s.inputState->input2)){
    s.client.println("\"i2\" : " + String(s.inputState->input2) + ",");
  }
  else{
    s.client.println("\"i2\" : null,");
  }
  
  s.client.println("\"r\" : " + String(s.inputState->useRedundantSensor) + ",");
  s.client.println("\"e\" : " + String(s.inputState->error) + ",");
  s.client.println("\"a\" : " + String(s.inputState->useAverage) + "");
  s.client.println("},");
  s.client.println("\"SP\" : " + String(s._settings->SetPoint) + ",");
  s.client.println("\"D\" : " + String(s._settings->PidDirection) + ",");
  s.client.println("\"Md\" : " + String(s._settings->OperatingMode) + ",");
  s.client.println("\"O\" : " + String(s._pidState->PidOutputSum) + ",");
  s.client.println("\"P\" : " + String(s._pidState->PidOutputP) + ",");
  s.client.println("\"I\" : " + String(s._pidState->PidOutputI) + ",");
  s.client.println("\"D\" : " + String(s._pidState->PidOutputD) + ",");
  s.client.println("\"Pr\" : " + String(s._pidState->UsePrimaryPID) + ",");
  s.client.println("\"Mn\" : " + String(s._pidState->ManualOutput) + "");
  s.client.println("}");






}