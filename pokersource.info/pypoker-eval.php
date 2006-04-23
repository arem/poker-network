<?php
$root = ".";
$title = "pypoker-eval";
require_once('header.php')
?>

<blockquote>

<h3>pypoker-eval: python interface to poker-eval</h3>

<p>
This package is python adaptor for the poker-eval toolkit for writing
programs which simulate or analyze poker games as found at
http://pokersource.org/poker-eval/. The python interface is
somewhat simpler than the C API of poker-eval. It assumes that the
caller is willing to have a higher level API and is not interested in
a one to one mapping of the poker-eval API.
</p>

<p>
For more information about the functions provided, check the
documentation of the PokerEval class in the pokereval.py file.
</p>

</blockquote>

<?php require_once('footer.php') ?>
