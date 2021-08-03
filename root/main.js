const image = ['bg.jpg', 'bg2.png'];
const rand = getRandomInt(2);

const body = document.body;

body.style.backgroundImage = `url(${image[rand]})`;

const signs = document.getElementsByClassName("signs");
const links = document.getElementsByTagName("a");

if (rand === 1){
    for (let link of links) {
        link.style.color = "aqua";
    }
    for (let sign of signs) {
        sign.style.color = "navajowhite";
    }
}
else {
    for (let link of links) {
        link.style.color = "yellow";
    }
    for (let sign of signs) {
        sign.style.color = "darkblue";
    }
}

function getRandomInt(max) {
    return Math.floor(Math.random() * max);
}