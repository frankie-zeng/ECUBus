var crypto = require('crypto');
var bufferTools = require('./buffer-tools.js');

var const_Zero = Buffer.from('00000000000000000000000000000000', 'hex');
var const_Rb = Buffer.from('00000000000000000000000000000087', 'hex');
var const_blockSize = 16;

exports.generateSubkeys = function (key) {
  var l = aes(key, const_Zero);

  var subkey1 = bufferTools.bitShiftLeft(l);
  if (l[0] & 0x80) {
    subkey1 = bufferTools.xor(subkey1, const_Rb);
  }

  var subkey2 = bufferTools.bitShiftLeft(subkey1);
  if (subkey1[0] & 0x80) {
    subkey2 = bufferTools.xor(subkey2, const_Rb);
  }

  return { subkey1: subkey1, subkey2: subkey2 };
};

function aes(key, message) {
  var keyLengthToCipher = { 16: 'AES-128-ECB', 24: 'AES-192-ECB', 32: 'AES-256-ECB' };
  if (!keyLengthToCipher[key.length]) {
    throw new Error('Keys must be 128, 192, or 256 bits in length.');
  }
  var cipher = crypto.createCipheriv(keyLengthToCipher[key.length], key, null);
  var result = cipher.update(message);
  cipher.final();
  return result;
}

exports.aesCmac = function (key, message) {
  var subkeys = exports.generateSubkeys(key);
  var blockCount = Math.ceil(message.length / const_blockSize);
  var lastBlockCompleteFlag, lastBlock, lastBlockIndex;

  if (blockCount === 0) {
    blockCount = 1;
    lastBlockCompleteFlag = false
  } else {
    lastBlockCompleteFlag = (message.length % const_blockSize === 0);
  }
  lastBlockIndex = blockCount -1;

  if (lastBlockCompleteFlag) {
    lastBlock = bufferTools.xor(getMessageBlock(message, lastBlockIndex), subkeys.subkey1);
  } else {
    lastBlock = bufferTools.xor(getPaddedMessageBlock(message, lastBlockIndex), subkeys.subkey2);
  }

  var x = Buffer.from('00000000000000000000000000000000', 'hex');
  var y;

  for (var index = 0; index < lastBlockIndex; index++) {
    y = bufferTools.xor(x, getMessageBlock(message, index));
    x = aes(key, y);
  }
  y = bufferTools.xor(lastBlock, x);
  return aes(key, y);
};

function getMessageBlock(message, blockIndex) {
  var block = Buffer.alloc(const_blockSize);
  var start = blockIndex * const_blockSize;
  var end = start + const_blockSize;

  message.copy(block, 0, start, end);

  return block;
}

function getPaddedMessageBlock(message, blockIndex) {
  var block = Buffer.alloc(const_blockSize);
  var start = blockIndex * const_blockSize;
  var end = message.length;

  block.fill(0);
  message.copy(block, 0, start, end);
  block[end - start] = 0x80;

  return block;
}
