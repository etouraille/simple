--TEST--
simple_test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('simple')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = simple_test1();

var_dump($ret);
?>
--EXPECT--
The extension simple is loaded and working!
NULL
