import requests

url = "https://api-v2.acrcloud.com/api/buckets/22150"

payload = {}
headers = {
    "Accept": "application/json",
    "Authorization": "Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiJ9.eyJhdWQiOiI3IiwianRpIjoiYTM3NzUxMjAwYTBiNjAwNDRjNjA0N2I3ZDBhYTY0NDk1MWUxYzM4NjUxNTRjZTdlYzY2NGI2YThhZDQ5N2NhMGM2MzhlMzkzYjNjMjJjNWYiLCJpYXQiOjE3MTgwNDUzOTEuNzg3NjA0LCJuYmYiOjE3MTgwNDUzOTEuNzg3NjA3LCJleHAiOjIwMzM1NzgxOTEuNzA3Njg0LCJzdWIiOiIxNzkwMzgiLCJzY29wZXMiOlsiKiIsIndyaXRlLWFsbCIsInJlYWQtYWxsIl19.W711SmqrASreiBT7CW53Tj7TdIg3apu6XsvWFpVc0ztZim5qUASYE-4a68X8f4TpYrQRhAvtd5f6ayRvYSXwJ2Ru2j2RcLVGYQpRIUPsgSLXC8pJp2Sanj_gV68xThdhZM2D_bXHGhSYmZ1tLTD86aHJZAGL4vtt_R3qOMpiOUkny7Ic8OimnZc3ddve7zCHkVU_S36eJwsLlqYTycZvwuIe9Sg7HtNJQvy7v8t8x0jrvedxMFDYaQ8mi3w-W6HT_atjdSJIG-mMSg5yCV5U-McWyV1y-4FawyET4_tzLudWS_FaxDoi2F64T_bbC2WaQQi1EIquZG9Tf9Q9XNGMKtxZysbypMZWGezFcrBk4DutWU33LYeV709JscxMsNEA4uKgehzOuloMRfv4hcEYgbIztE8z6YBwzAbzVdU-5HndrKD7C4qx_iJ28GaN1yxZAnMMYtvW9R7n8cpSRpuBpQ-gLyy42UPEA7Xoq89PyIDG4PXSe_851iFVWUZvYFrkRDOKIvl2V_wqjeBIzuo9AfloEuX9ICJgNHZb8Fz48KwUAZPXhV--7Ye3fbKob5Zt73lu0yGtxS5PfQKUVYhZs-BMWDepvGhpLNCoMwqItrc6oYSrV4Cz5xAiWFb0EvS5JYG9Tq5CqR5frg9RcdPsJ8VTTEcgVGfByWqADL0gpgI",
}

# response = requests.request("GET", url, headers=headers, data=payload)
response = requests.get(url, headers=headers)

print(response.text)
