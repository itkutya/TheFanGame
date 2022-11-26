<?php
$host = "localhost";
$user = "id19916612_thefangameadmindb";
$password = "jOsW@15=7W{{csr<";
$db = "id19916612_thefangamedb";

$mysql = mysqli_connect($host, $user, $password, $db);
if(mysqli_connect_errno())
    echo mysqli_connect_errno();
else
{
    $name = $_POST['username'];
    $pw = $_POST['password'];
    $resoult = mysqli_query($mysql, "SELECT * FROM `TheFanGameAccounts` WHERE Username = '$name' AND Password = '$pw'");
    if(mysqli_num_rows($resoult))
        echo "Success.";
    else
        echo "Failed.";
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
    <title>Login to the game.</title>
</head>
<body>
</body>
</html>