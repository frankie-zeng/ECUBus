var crypto = require('crypto');
var bufferTools = require('./buffer-tools.js');

var const_Zero = Buffer.from('0000000000000000', 'hex');
var const_Rb = Buffer.from('000000000000001b', 'hex');
var const_blockSize = 8;

exports.generateSubkeys = function (key) {
  var l = tdes(key, const_Zero);

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

function tdes(key, message) {
  
  if (key.length!=24) {
    throw new Error('Keys must be 192 bits in length.');
  }
   var key1=key.slice(0,8);
   var key2=key.slice(8,16);
   var key3=key.slice(16,24);
   var cipher1 = crypto.createCipheriv('des-ecb',key1,null);
   var cipher2 = crypto.createDecipheriv('des-ecb',key2,null);
   var cipher3 = crypto.createCipheriv('des-ecb',key3,null);
   var result1=cipher1.update(message);
   cipher1.final();
   cipher2.setAutoPadding(false);
   var result2=cipher2.update(result1);
   cipher2.final();
   var result3=cipher3.update(result2);
   cipher3.final();
   return result3;
}

exports.tdesCmac = function (key, message) {
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

  var x = Buffer.from('0000000000000000', 'hex');
  var y;

  for (var index = 0; index < lastBlockIndex; index++) {
    y = bufferTools.xor(x, getMessageBlock(message, index));
    x = tdes(key, y);
  }
  y = bufferTools.xor(lastBlock, x);
  return tdes(key, y);
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
