setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("current").innerHTML = this.responseText + " bar";
    }
  };
  xhttp.open("GET", "/current", true);
  xhttp.send();
}, 1000);

setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("prev").innerHTML = this.responseText + " bar";
    }
  };
  xhttp.open("GET", "/prev", true);
  xhttp.send();
}, 1000);

setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("max").innerHTML = this.responseText + " bar";
    }
  };
  xhttp.open("GET", "/max", true);
  xhttp.send();
}, 1000);

setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("min").innerHTML = this.responseText + " bar";
    }
  };
  xhttp.open("GET", "/min", true);
  xhttp.send();
}, 1000);

//DATE n TIME
function display_hari() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("hari").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/hari", true);
  xhttp.send();
}

function display_tanggal() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("tanggal").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/tanggal", true);
  xhttp.send();
}

setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("jam").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/jam", true);
  xhttp.send();
}, 1000);