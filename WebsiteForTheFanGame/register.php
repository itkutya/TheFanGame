<?php
$host = "localhost";
$user = "id19916612_thefangameadmindb";
$password = "jOsW@15=7W{{csr<";
$db = "id19916612_thefangamedb";

$mysqli = new mysqli($host, $user, $password, $db);
if(mysqli_connect_errno())
    echo $mysqli->connect_error;
else
{
    if(isset($_POST['username']) && isset($_POST['password']) && isset($_POST['email']))
    {
        $name = $_POST['username'];
        $pw = $_POST['password'];
        $email = $_POST['email'];
        $query ="SELECT * FROM `TheFanGameAccounts` WHERE Username='$name'";
        $resoult = $mysqli->query($query);
        if($resoult->num_rows)
            echo "Name already exist.";
        else 
        {
            $salt = time();
            $pw = ($pw - ($salt * 352));
            $query ="INSERT INTO `TheFanGameAccounts` (`Username`, `Email`, `Salt`, `Password`) VALUES ('$name', '$email', '$salt', '$pw')";
            $resoult = $mysqli->query($query);
            if($resoult)
                echo "Success.";
            else
                echo "Query failed.";
        }
    }
    else
        echo "Values not set.";
}
exit();
?>