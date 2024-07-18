from django.db import models


class City(models.Model):
    name = models.CharField(verbose_name="城市名", max_length=10, unique=True)
    def __str__(self):
        return self.name


class Distance(models.Model):
    cityA = models.ForeignKey(City, on_delete=models.CASCADE,related_name="asA")
    cityB = models.ForeignKey(City, on_delete=models.CASCADE,related_name="asB")
    distance = models.IntegerField(verbose_name="距离")
    def __str__(self):
        res = "the distance between " + str(self.cityA) + " and " + str(self.cityB) + ": " + str(self.distance)
        return res


class FlightInfo(models.Model):
    price = models.IntegerField(verbose_name="票价")
    departure_city = models.ForeignKey(City, on_delete=models.CASCADE, related_name="asDeparture")
    destination_city = models.ForeignKey(City, on_delete=models.CASCADE, related_name="asDestination")
    mileage = models.IntegerField(verbose_name="里程")
    depart_time = models.DateTimeField(verbose_name="出发时间")
    arrive_time = models.DateTimeField(verbose_name="到达时间")
    time_cost = models.DurationField(verbose_name="耗时", null=True, blank=True)

    """
    def __str__(self):
        res = "from: " + str(self.departure_city) + " to: " + str(self.destination_city) + "\n" \
            "$" + str(self.price) + "  " + str(self.depart_time) + "-" + str(self.arrive_time)
        return res
    """