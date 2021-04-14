exports.bitShiftLeft = function (buffer) {
  var shifted = Buffer.alloc(buffer.length);
  var last = buffer.length - 1;
  for (var index = 0; index < last; index++) {
    shifted[index] = buffer[index] << 1;
    if (buffer[index + 1] & 0x80) {
      shifted[index] += 0x01;
    }
  }
  shifted[last] = buffer[last] << 1;
  return shifted;
}

exports.xor = function (bufferA, bufferB) {
  var length = Math.min(bufferA.length, bufferB.length);
  var output = Buffer.alloc(length);
  for (var index = 0; index < length; index++) {
    output[index] = bufferA[index] ^ bufferB[index];
  }
  return output;
}

var bitmasks = [0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01];

exports.toBinaryString = function (buffer) {
  var binary = '';
  for (var bufferIndex = 0; bufferIndex < buffer.length; bufferIndex++) {
    for (var bitmaskIndex = 0; bitmaskIndex < bitmasks.length; bitmaskIndex++) {
      binary += (buffer[bufferIndex] & bitmasks[bitmaskIndex]) ? '1' : '0';
    }
  }
  return binary;
}
