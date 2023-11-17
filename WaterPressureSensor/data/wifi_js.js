var ssid_list = "";
var rssi_list = "";

//buat ngatur layout aja
const json =
  '{"ssid":["AlFA12AJfgadfdcvzcvdfvzdvzsdvsDvADvADSV","Aretha Artha","ASRWPD","namawifiapa","sdasda","adfasdf"],"rssi":[-77,-88,-88,-93, -80, -50], "encrypt":["o", "o", "l","l","l"]}';

document.getElementById("scan-button").addEventListener("click", function () {
  console.log("Scan WiFi button clicked");
  updateWiFiList();
});

function updateWiFiList() {
  function calculateSignalStrength(rssi) {
    if (rssi >= -77) return "3";
    if (rssi >= -88) return "2";
    return "1";
  }

  function encryptionType(encrypt) {
    return encrypt === "o" ? "open" : "locked";
  }

  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      // const data = JSON.parse(json);
      const data = JSON.parse(this.responseText);
      var tableElement = document.getElementById("wifi_list");

      // Kosongkan tabel sebelumnya
      tableElement.innerHTML = "";

      data.ssid.forEach((ssid, i) => {
        var row = document.createElement("tr");
        var ssidCell = document.createElement("td");
        var rssiCell = document.createElement("td");

        var link = document.createElement("a");
        link.href = "#";
        link.onclick = function () {
          c(link);
        }; // Gunakan fungsi anonim untuk meneruskan elemen link
        link.textContent = ssid;
        ssidCell.appendChild(link);

        var signalStrengthValue = calculateSignalStrength(data.rssi[i]);
        var enkripsi = encryptionType(data.encrypt[i]);
        var imgSrc = `/img/network_wifi_${signalStrengthValue}${
          enkripsi === "locked" ? "_locked" : ""
        }.png`;

        var img = document.createElement("img");
        img.src = imgSrc;
        rssiCell.appendChild(img);

        row.appendChild(ssidCell);
        row.appendChild(rssiCell);
        tableElement.appendChild(row);
      });
    }
  };
  xhr.open("GET", "/scan", true);
  // xhr.open("GET", "/wifiManager.html", true);
  xhr.send();
}

updateWiFiList();

//Untuk Keperluan Form
function c(l) {
  document.getElementById("ssid").value = l.textContent || l.innerText;
  document.getElementById("pass").focus(); // Baris ini akan mengatur fokus ke elemen input password
}
function f() {
  var x = document.getElementById("pass");
  x.type === "password" ? (x.type = "text") : (x.type = "password");
}
