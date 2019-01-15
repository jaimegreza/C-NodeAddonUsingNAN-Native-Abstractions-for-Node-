function showMessage(message)
{
    var toast = document.getElementById("messageToast");
    toast.innerHTML = message;
    toast.className = "show";
    setTimeout(function(){ toast.className = toast.className.replace("show", ""); }, 10000);
}

export const showMessageToast = (message)=>{
    showMessage(message);
}