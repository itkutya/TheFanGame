<?php
$host = "localhost";
$user = "id19916612_thefangameadmindb";
$password = "jOsW@15=7W{{csr<";
$db = "id19916612_thefangamedb";

//General query to change any stored data in the db.
$mysqli = new mysqli($host, $user, $password, $db);
if($mysqli->connect_error)
    echo $mysqli->connect_error;
else
{
    $query = "SELECT * FROM `TheFanGameCharacters`";   
    $resoult = $mysqli->query($query);
    if($resoult->num_rows)
    {
        echo "Success.#";
        while($column = $resoult->fetch_row())
        {
            echo $column[0]."#";
            echo $column[1]."#";
            echo $column[2]."#";
        }
    }
    else
        echo "Failed.";
}
exit();
?>