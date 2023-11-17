// Toggle class active untuk hamburger menu
var navbarNav = document.querySelector('.navbar-nav');
var navbarEx = document.querySelector('.navbar-extra');
var hamburger = document.querySelector('.hamburger-menu');
// ketika hamburger menu di klik
hamburger.addEventListener('click', () => {
  hamburger.classList.toggle('active');
  navbarNav.classList.toggle('active');
  checkbox.checked == true;
});


// Klik di luar elemen
const hm = document.querySelector('#hamburger-menu');

document.addEventListener('click', function (e) {
  if (!hamburger.contains(e.target) && !navbarNav.contains(e.target)) {
    hamburger.classList.remove('active');
    navbarNav.classList.remove('active');
  }
});





