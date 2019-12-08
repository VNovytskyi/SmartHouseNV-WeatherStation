function hamButtonClick()
{
    var hamButton = document.getElementById("hamButton");
    hamButton.classList.toggle('active');

    var modal = document.getElementById('id01');
            
    document.getElementById('id01').style.display='block';
 
    window.onclick = function(event) {
        if (event.target == modal) {
            modal.style.display = "none";
        }
    }
}

function closeForm()
{
    document.getElementById('id01').style.display='none';
}