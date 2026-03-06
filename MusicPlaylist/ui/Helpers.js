.pragma library

function formatTime(seconds) {
    if (isNaN(seconds)) return "0:00";
    let min = Math.floor(seconds / 60);
    let sec = Math.floor(seconds % 60);
    return min + ":" + (sec < 10 ? "0" + sec : sec);
}