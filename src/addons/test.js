const HSE = require('./../../build/Release/HSE.node')
const hse = new HSE.HSE()
const symkey = Buffer.from('000102030405060708090a0b0c0d0e0f','hex')
const info={
    offset:0x400000,
    handle:50,
    flags:0,
    counter:0,
    smr:3,
    type:5
}
var keyMaterial=hse.importSymKey(info,symkey)
console.log(keyMaterial)
const rsaPubKey =
`-----BEGIN PUBLIC KEY-----\r\n
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAx0KfhKB3VgKSUdyrdVE6\r\n
J7iSW0EIDAsv81bpKOjtA/Tj9GvSab0eP8+JwFsHTB+ES/Nc0FzAcITsqP4W/XXo\r\n
jgcT6M8ROZe0IuynZjI6zkOQFzolPkFJanSCjPgigPNtJDVJoKMmStEM3sj5xB+Q\r\n
VMuo/3agd1SBatS9XW6M545znl+YxW42qmjNZXK2/g9xvBF79yEJncGdUa2TnA3Y\r\n
tjAx0OPHAsDsFvgpkXe1YAXZ8Og/PHKqeheSkJP5cEKqfF4JIfqJdvQ9jR08ypas\r\n
x0lhmbgbVO9LvJ36RflGXHYYCfYx4j0d/O6voKmkxFS2wFgqcDN8En8YgWZTxoQb\r\n
6QIDAQAB\r\n
-----END PUBLIC KEY-----\r\n
`
keyMaterial=hse.importPubKey(info,rsaPubKey)
console.log(keyMaterial)

const eccPubKey =
`-----BEGIN PUBLIC KEY-----\r\n
MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE9rWjXOufGTEg3bP8BE5xcNOhfUqe\r\n
LM6qj6K32396GjximBW0jPM9VjPsx0JC7+V4mfO63n5jpvsiT8VPvTXTRA==\r\n
-----END PUBLIC KEY-----\r\n
`
keyMaterial=hse.importPubKey(info,eccPubKey)
console.log(keyMaterial)
const rsaPrivKey =
`
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEAx0KfhKB3VgKSUdyrdVE6J7iSW0EIDAsv81bpKOjtA/Tj9GvS
ab0eP8+JwFsHTB+ES/Nc0FzAcITsqP4W/XXojgcT6M8ROZe0IuynZjI6zkOQFzol
PkFJanSCjPgigPNtJDVJoKMmStEM3sj5xB+QVMuo/3agd1SBatS9XW6M545znl+Y
xW42qmjNZXK2/g9xvBF79yEJncGdUa2TnA3YtjAx0OPHAsDsFvgpkXe1YAXZ8Og/
PHKqeheSkJP5cEKqfF4JIfqJdvQ9jR08ypasx0lhmbgbVO9LvJ36RflGXHYYCfYx
4j0d/O6voKmkxFS2wFgqcDN8En8YgWZTxoQb6QIDAQABAoIBAQC0fqgKtmSC979d
AfwaQCLiujFxEd5dMoET9wUrdprD+7/FtymZeKL/AFrycEKS9PWc6uyHHY2AHxhV
GBzxKS4oAUdKms8qkT2uaRpV22gjUVYjhBsaYn9DPBjJ4zc2Acs6gj6YkVDtcqIq
6Hg+7KyL8xO5nNZMYbL9d4m1KZ/fWzL5rHjPr7HcL8HbP5BtEby3zKz/2cLyzpPJ
s+QFjTl981l0AS4g7ezhbNWPuBG0bDl6AnSFYVyQd6DNO44Xp62pSBvwmEIfus6K
yh3UfOnLQy/h6m3Vax/JKzr4jzFfCxsBCGHqn2rGIuvqPylAfv0+YxBMBmR6qYNm
57Uihp8BAoGBAPQwo9xp45HLrvpDto2bzx+he4OL7z9NNXvGziQDbgyw3nRsvXs8
GJS2GCUl7hQme20If7JWaYm6BQpg+Xz00w3q5ib077UUTlLqe7Vi9StRu/MdQ0G9
JP2VbGXQotBYimful2UQrtfOsmXYgCFkvtS8KSjzPvsUXjpx3m5uSKiJAoGBANDl
syKLrGB3//Jq4xuEWi2frdpRCpi7zho4Dm2lJoAQJe8FHvOaEW/1AQT3IOdT6kM0
872disBy2MfJ6vK6nvEtA55U6/EZ3YXFrs/9wZrz6WZFUWZLpMzfjIjSAisttYrb
S+HYaS/Ff+IkviJ25HeWR2Ss0Hw+nnM5zubxikBhAoGBALEVwQ9sPxuRMcDOcEn7
r1ZJ/k+koIkwjdPIld/qGNndPkYCQdtn9o+KCrKN8jzst1+X5TtIjVrS6yrerq9e
wNv7+JJR2LBiHHbnE3W0YQXcTUf+dbLr3zKDNUwU5K3hN+YtUd96HMHmZNuVCT6u
V/Hgzpyi4o6k87zArUU6dzi5AoGAAzNBp0nUfjXYafJRQF6qswA7PZj4PJbD9Yre
Tbe1+0hEDIFblAfMNJbgSR8wFdsnP5hUE5eSRJh90bG6M7XAMZmq8IYEh1EKH76D
ixQsHx+Vd8egE50dDTZvhLkS525v1Yy+gNs74Ut5RhGiCA0iOuLGfThYiU68Rq+g
B9GzeIECgYEA0rS5cVc694gHeKuuUufrBMJJ6HMvWtS2/RNLJ0EhOcVfsPo5KP7a
ncFcn1DP0B+bxSRr7Viz5IKI6j2ZeJb3zMVOn7AS5h7Mcf2u4pnSin7C56s9ksDJ
y3Uv31Ek6iHz/mUmz1uC28ich/lcaNwcYYyODiWE/upq6KEFfwL0QCU=
-----END RSA PRIVATE KEY-----`
keyMaterial=hse.importPrivKey(info,rsaPrivKey)
console.log(keyMaterial)


const  eccPrivKey=
`
-----BEGIN EC PRIVATE KEY-----\r\n
MHcCAQEEIEl6YGbmsH3RUjbzbSxf8Yh1C13MgpkrJpEN3NQHAb6foAoGCCqGSM49\r\n
AwEHoUQDQgAE9rWjXOufGTEg3bP8BE5xcNOhfUqeLM6qj6K32396GjximBW0jPM9\r\n
VjPsx0JC7+V4mfO63n5jpvsiT8VPvTXTRA==\r\n
-----END EC PRIVATE KEY-----\r\n`

keyMaterial=hse.importPrivKey(info,eccPrivKey)
console.log(keyMaterial)


const catalog=[
    {
        keyOwn:1,
        keyType:2,
        keySize:4,
        muMask:6,
        keyNum:10
    }
]

keyMaterial=hse.formatCatalog(5000,catalog,catalog)
console.log(keyMaterial)