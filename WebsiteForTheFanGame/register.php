<?php
$host = "localhost";
$user = "id19916612_thefangameadmindb";
$password = "jOsW@15=7W{{csr<";
$db = "id19916612_thefangamedb";

$mysqli = new mysqli($host, $user, $password, $db);
if($mysqli->connect_error)
    echo $mysqli->connect_error;
else
{
    if(isset($_POST['username']) && isset($_POST['password']) && isset($_POST['email']))
    {
        $name = $_POST['username'];
        $pw = $_POST['password'];
        $email = $_POST['email'];
        $query ="INSERT INTO `TheFanGameAccounts` (`Username`, `Password`, `Email`, `LoginBuffer`) VALUES ('$name', '$pw', '$email', '')";
        $resoult = $mysqli->query($query);
        if($resoult)
        {
            $query = "INSERT INTO `TheFanGameAccountsInfo` (`Username`, `Level`, `CurrentXP`, `MaxXP`) VALUES ('$name', '0', '0', '0')";
            $resoult = $mysqli->query($query);
            if($resoult)
                echo "Success.";
            else
                echo "Failed.";
        }
        else
            echo "Name already exists or query failed.";
    }
}
exit();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="icon" type="image/x-icon" href="favicon.ico">
    <title>Register to the game.</title>
</head>
<body>
</body>
</html>