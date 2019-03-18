--TEST--
Check if simple is loaded
--SKIPIF--
<?php
if (!extension_loaded('simple')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "simple" is available';
?>
--EXPECT--
The extension "simple" is available
